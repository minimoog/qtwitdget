var stateStack = [];

function goBack() {
    if (stateStack.length == 0)
        return;

    if (stateStack.length == 1) {
        timelines.state = '';
        stateStack.pop();
        return;
    }

    stateStack.pop();

    var stateSlot = stateStack[stateStack.length - 1];

    //set state
    timelines.state = stateSlot.statename;

    if (stateSlot.statename == 'userinfo') {
        userInfo.fetchByName(stateSlot.screenname);
        userTimelineListModel.fetch(stateSlot.screenname);
    } else if (stateSlot.statename == 'search') {
        if (stateSlot.hashtag != undefined)
            searchResultList.doSearch(stateSlot.hashtag);
    } else if (stateSlot.statename == 'conversation') {
        conversationList.model.followConversation(stateSlot.id);
    }
}

function showUser(screenname) {
    // ### TODO Shouldn't be separated
    userInfo.fetchByName(screenname);
    userTimelineListModel.fetch(screenname);
    timelines.state = 'userinfo';

    //push on stack
    var stateSlot = {};
    stateSlot.statename = 'userinfo';
    stateSlot.screenname = screenname;

    stateStack.push(stateSlot);
}

function searchHashtag(hashtag) {
    searchResultList.doSearch(hashtag);
    timelines.state = 'search';

    var stateSlot = {};
    stateSlot.statename = 'search';
    stateSlot.hashtag = hashtag;

    stateStack.push(stateSlot);
}

function showConversation(id) {
    conversationList.model.followConversation(id);
    timelines.state = 'conversation';

    var stateSlot = {};
    stateSlot.statename = 'conversation';
    stateSlot.id = id;

    stateStack.push(stateSlot);
}

function showHometimeline() {

    //avoid pushing duplicate state
    if (timelines.state != '') {
        var stateSlot = {};
        stateSlot.statename = '';

        stateStack.push(stateSlot);
    }

    homeTimelineModel.showNewTweets();
    timelines.state = '';   //default state
}

function showMentions() {
    if (timelines.state != 'mentions') {
        var stateSlot = {};
        stateSlot.statename = 'mentions';

        stateStack.push(stateSlot);
    }

    mentionsModel.showNewTweets();
    timelines.state = 'mentions';
}

function showDirectMessages() {
    if (timelines.state != 'directMessages') {

        var stateSlot = {};
        stateSlot.statename = 'directMessages';

        stateStack.push(stateSlot);
    }

    timelines.state = 'directMessages';
}

function showSearch() {         // ### TODO: Needs to remember search query?
    timelines.state = 'search';

    var stateSlot = {};
    stateSlot.statename = 'search';

    stateStack.push(stateSlot);
}
