var stateStack = [];

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

    var stateSlot = {}
    stateSlot.statename = 'conversation';
    stateSlot.id = id;

    stateStack.push(stateSlot);
}
