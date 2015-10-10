(function(exports) {
  _launchContext = {
    get initialPanelId() {
      return initialPanelId;
    },
    get initialPanelHandler() {
      return initialPanelHandler;
    },
    get activityHandler() {
      return activityHandler;
    }
  }

  Object.defineProperty(exports, 'LaunchContext', {
    configurable: true,
    get: function() {
      return _launchContext;
    }
  });
})(window);
