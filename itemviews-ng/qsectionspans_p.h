/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Itemviews NG project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTSECTIONSPANS_P_H
#define QTSECTIONSPANS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qdebug.h>

QT_BEGIN_NAMESPACE

template <typename T>
class QtSectionSpans
{
public:
    QtSectionSpans() : cachedSpanStart(-1), cachedSectionStart(-1) {}

    struct Span {
        inline Span(T v, uint c = 1) : value(v), count(c) {}
        T value;
        uint count;
    };

    void setValue(int index, T value)
    {
        int spanFirstSection, i;
        useCache(index, &i, &spanFirstSection);
        invalidateCache();
        // create span
        for (; i < spans.count(); ++i) {
            int spanSectionCount = spans.at(i).count;
            int spanLastSection = spanFirstSection + spanSectionCount - 1;
            // can we replace the span ?
            if (spanFirstSection == index && spanSectionCount == 1) {
                int prev = i - 1;
                int next = i + 1;
                bool mergeWithPrev = (prev >= 0 && spans.at(prev).value == value);
                bool mergeWithNext = (next < spans.count() && spans.at(next).value == value);
                if (mergeWithPrev && mergeWithNext) {
                    spans[prev].count += spans.at(next).count + 1;
                    // remove the replaced span
                    spans.removeAt(i);
                    // remove the next span
                    spans.removeAt(i);
                    return;
                }
                if (mergeWithPrev) {
                    spans[prev].count += 1;
                    spans.removeAt(i);
                    return;
                }
                if (mergeWithNext) {
                    spans[next].count += 1;
                    spans.removeAt(i);
                    return;
                }
                // replace the span; count is the same
                spans[i].value = value;
                return;
            }
            // is it part of the beginning of the span ?
            if (spanFirstSection == index) {
                spans[i].count -= 1;
                int prev = i - 1;
                bool mergeWithPrev = (prev >= 0 && spans.at(prev).value == value);
                if (mergeWithPrev) {
                    spans[prev].count += 1;
                } else { // we have to create a new span
                    spans.insert(i, Span(value));
                }
                return;
            }
            // is it part of the end of the span ?
            if (spanLastSection == index) {
                spans[i].count -= 1;
                int next = i + 1;
                bool mergeWithNext = (next < spans.count() && spans.at(next).value == value);
                if (mergeWithNext) {
                    spans[next].count += 1;
                } else { // we have to create a new span
                    spans.insert(next, Span(value));
                }
                return;
            }
            // is it in the middle of the span ?
            if (spanFirstSection < index && spanLastSection > index) {
                T spanSectionValue = spans.at(i).value;
                // first span
                spans[i].count = index - spanFirstSection;
                // middle span
                spans.insert(i + 1, Span(value));
                // last span
                int lastSpanCount = spanLastSection - index;
                spans.insert(i + 2, Span(spanSectionValue, lastSpanCount));
                return;
            }
            // next span
            spanFirstSection += spanSectionCount;
        }
        qWarning() << "setValue: no section span found for index" << index;
    }

    T sectionValue(int index, T defaultValue) const
    {
        int sectionStart, i;
        useCache(index, &i, &sectionStart);
        // get value
        for (; i < spans.count(); ++i) {
            const Span &span = spans.at(i);
            int sectionEnd = sectionStart + span.count - 1;
            if (index >= sectionStart && index <= sectionEnd) {
                setCache(i, sectionStart);
                return span.value;
            }
            sectionStart = sectionEnd + 1;
        }
        return defaultValue;
    }

    void addSections(T value, uint count)
    {
        invalidateCache();
        // The value should be the default value of T
        int i = spans.count() - 1;
        // can we merge with the last span ?
        if (!spans.isEmpty() && spans.at(i).value == value) {
            spans[i].count += count;
        } else { // we need to add a new span
            spans.append(Span(value, count));
        }
    }

    void removeSections(int removeCount)
    {
        invalidateCache();
        for (int i = spans.count() - 1; i >= 0 && removeCount > 0; --i) {
            uint sectionSpanCount = spans.at(i).count;
            // can we remove the whole span ?
            if (sectionSpanCount <= uint(removeCount)) {
                spans.removeAt(i);
            } else { // remove part of the span
                spans[i].count -= removeCount;
            }
            removeCount -= sectionSpanCount;
        }
    }

    void insertSectionsAt(T value, int index, int count)
    {
        int spanFirstSection, i;
        useCache(index, &i, &spanFirstSection);
        invalidateCache();
        for (; i < spans.count(); ++i) {
            T spanValue = spans.at(i).value;
            uint spanSectionCount = spans.at(i).count;
            int spanLastSection = spanFirstSection + spanSectionCount - 1;
            // is it the correct span ?
            if (index >= spanFirstSection && index <= spanLastSection) {
                // can we add to the span ?
                if (spanValue == value) {
                    spans[i].count += count;
                } else if (index == spanFirstSection) {
                    // insert before
                    spans.insert(i, Span(value, count));
                } else {
                    // split the span
                    spans[i].count = index - spanFirstSection;
                    spans.insert(i + 1, Span(value, count));
                    spans.insert(i + 2, Span(spanValue, spanSectionCount - index + 1));
                }
                return; // done
            }
            spanFirstSection += spanSectionCount;
        }
        qWarning() << "insertSectionsAt: no section found for index" << index;
    }

    void removeSectionsAt(int index, int removeCount)
    {
        int spanFirstSection, i;
        useCache(index, &i, &spanFirstSection);
        invalidateCache();
        for (; i < spans.count(); ++i) {
            uint spanSectionCount = spans.at(i).count;
            int spanLastSection = spanFirstSection + spanSectionCount - 1;
            if (index >= spanFirstSection && index <= spanLastSection) {
                // can we remove the span ?
                if (index == spanFirstSection && spanSectionCount <= uint(removeCount)) {
                    int prev = qMax(i - 1, 0); // if 'i' is 0, it doesn't matter
                    int next = qMin(i + 1, spans.count() - 1);
                    bool merge = (spans.at(prev).value == spans.at(next).value);
                    if (merge) {
                        spans[prev].count += spans.at(next).count;
                        // remove the span
                        spans.removeAt(i);
                        // remove the next span
                        spans.removeAt(i--);
                    } else {
                        // remove the span
                        spans.removeAt(i--);
                    }
                    removeCount -= spanSectionCount;
                    if (removeCount <= 0)
                        return; // done
                } else { // remove part of the span
                    int spanLocalIndex = (index - spanFirstSection);
                    uint spanRemoveCount = qMin<uint>(spanSectionCount - spanLocalIndex + 1, removeCount);
                    spans[i].count -= spanRemoveCount;
                    spanFirstSection += spans.at(i).count;
                    removeCount -= spanRemoveCount;
                    if (removeCount <= 0)
                        return; // done
                }
            } else { // nothing removed yet
                spanFirstSection += spanSectionCount;
            }
        }
        qWarning() << "removeSectionsAt: no section found for index" << index;
    }

    void replaceAll(T value)
    {
        int count = sectionCount();
        clear();
        spans.append(Span(value, count));
    }

    int sectionCount() const
    {
        int count = 0;
        for (int i = 0; i < spans.count(); ++i)
            count += spans.at(i).count;
        return count;
    }

    // direct QList access

    inline const Span &at(int i) const
    {
        return spans.at(i);
    }

    inline Span &operator[](int i)
    {
        invalidateCache();
        return spans[i];
    }

    inline int count() const
    {
        return spans.count();
    }

    inline void clear()
    {
        invalidateCache();
        spans.clear();
    }

    // caching

    inline void invalidateCache()
    {
        cachedSpanStart = -1;
        cachedSectionStart = -1;
    }

    inline void useCache(int index, int *spanStart, int *sectionStart) const
    {
        const bool useCache = (cachedSectionStart <= index && cachedSectionStart != -1);
        *sectionStart = (useCache ? cachedSectionStart : 0);
        *spanStart = (useCache ? cachedSpanStart : 0);
    }

    inline void setCache(int spanStart, int sectionStart) const
    {
        cachedSpanStart = spanStart;
        cachedSectionStart = sectionStart;
    }

    QList<Span> spans;
    mutable int cachedSpanStart;
    mutable int cachedSectionStart;
};

QT_END_NAMESPACE

#endif//QTSECTIONSPANS_P_H
