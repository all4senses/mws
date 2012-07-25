/**
 * Devel helpers and sanity checks.
 *
 * This file will be included only when fb_devel.module is enabled and user
 * has 'access devel information' permission.
 */

FB_Devel = function(){};

FB_Devel.sanityCheck = function() {

  if (Drupal.settings.fb_devel.verbose == 'extreme') { // Expensive test, only when extreme.
    if (typeof(FB) != 'undefined' &&
        FB.getAccessToken() && // Unfortunately, we can only check when access token known (user logged in.)
        Drupal.settings.fb.fb_init_settings.appId) {
      // Test, was FB initialized with the right app id?
      FB.api('/app', function(response) {
        if (response.id != Drupal.settings.fb.fb_init_settings.appId) {
          alert("fb_devel.js: Facebook JS SDK initialized with app id: " + response.id + ".  Expected " + Drupal.settings.fb.fb_init_settings.appId + "!");
          debugger;
          // If you're here, you probably have multiple facebook modules installed, and they are competing to initialize facebook's javascript API.
        }
      });
    }
  }

  var root = jQuery('#fb-root');
  if (root.length != 1) {
    debugger; // not verbose.
    if (Drupal.settings.fb_devel.verbose) {
      alert("fb_devel.js: no <div id=fb-root> found!"); // verbose
    }
  }
};

/**
 * Called when fb.js triggers the 'fb_init' event.
 */
FB_Devel.initHandler = function() {
  FB_Devel.sanityCheck();

  // Facebook events that may be of interest...
  //FB.Event.subscribe('auth.login', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.logout', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.statusChange', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.sessionChange', FB_Devel.debugHandler);
};

// Helper, for debugging facebook events.
FB_JS.debugHandler = function(response) {
  debugger;
};

/**
 * Implements Drupal javascript behaviors.
 */
Drupal.behaviors.fb_devel = function(context) {
  jQuery(document).bind('fb_init', FB_Devel.initHandler);

  //FB_Devel.sanityCheck(); // This is now done in page footer.
};