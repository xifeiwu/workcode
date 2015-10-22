(function(exports) {
  // test Map
  TestMap = {
    _states: new Map(),
    _statesByState: new Map(),
  
    registerState: function(state, provider) { 
      this._states.set(provider.name, provider);
      this._statesByState.set(state, provider);
    },
  
    unregisterState: function(state, provider) { 
      this._states.delete(provider.name);
      var machine = this._statesByState.get(state);
      if (machine === provider) { 
        this._statesByState.delete(state);
      }
    },
    showStates: function(){
      for (var [key, value] of this._statesByState) {
        console.log(key + " = " + value);
      }
    },
  }
  
  TestMap.registerState('isFtuUpgrading', "this1");
  TestMap.registerState('isFtuRunning', "this2");
  TestMap.showStates();
  
})(window);
