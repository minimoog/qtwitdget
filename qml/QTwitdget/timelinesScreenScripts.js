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
    homeTimelineModel.showNewTweets();
    timelines.state = "";   //default state

    var stateSlot = {};
    stateSlot.statename = '';

    stateStack.push(stateSlot);
}

function showMentions() {
    mentionsModel.showNewTweets();
    timelines.state = 'mentions';

    var stateSlot = {};
    stateSlot.statename = 'mentions';

    stateStack.push(stateSlot);
}

function showDirectMessages() {
    dmList.model.showNewTweets();
    timelines.state = 'directMessages';

    var stateSlot = {};
    stateSlot.statename = 'directMessages';

    stateStack.push(stateSlot);
}

//function showSearch() {         // ### TODO: Needs to remember search query?
//    timelines.state = 'search';

//    var stateSlot = {};
//    stateSlot.statename = 'search';

//    stateStack.push(stateSlot);
//}
