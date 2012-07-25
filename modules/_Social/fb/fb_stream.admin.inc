<?php


/**
 * Form callback for general settings.
 */
function fb_stream_admin_settings() {
  $form = array();

  $token = variable_get(FB_STREAM_VAR_TOKEN, '');

  if ($token && empty($_POST) && empty($_REQUEST['code'])) {
    try {
      // TODO: consolodate graph api, use batch.
      $from = fb_graph('me', array('access_token' => $token));
      $via = fb_graph('app', array('access_token' => $token));
      // Show helpful messages about the current access token.
      drupal_set_message(t('The current token (%token) allows posting by %user_name via the %app_name application.', array(
                             '%app_name' => $via['name'],
                             '%user_name' => $from['name'],
                             '%token' => $token,
                           )));
    }
    catch (Exception $e) {
      fb_log_exception($e, t('Unable to query graph with fb_stream token.'));
      drupal_set_message(t('There was an error using the current access token, %token.  Consider generating a new token.', array(
                             '%token' => $token,
                           )), 'error');
    }
  }

  if (!empty($_REQUEST['code']) && empty($_POST)) {
    // Send user to this URL after token is generated.
    $redirect_uri = url($_REQUEST['q'], array(
                          'absolute' => TRUE,
                          'query' => array(
                            'client_id' => $_REQUEST['client_id'],
                          ),
                        ));

    $token = fb_stream_admin_code_to_token($_REQUEST['code'], $_REQUEST['client_id'], $redirect_uri);
    drupal_set_message(t('Generated a new access token %token.  Note you must press the save button to use this value!', array('%token' => $token)), 'warning');
  }

  $form['fb_stream_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Token'),
    '#default_value' => $token,
    '#description' => t('Use <a href=!url target=_blank>Facebook\'s Graph Explorer</a> to generate a token you can copy and paste into this form. <br/><em>For best results your token should include the publish_stream, manage_pages, and offline_access extended permissions</em>.', array(
                          '!url' => 'https://developers.facebook.com/tools/explorer',
                        )),
    '#required' => TRUE,
  );

  // TODO: show user what permissions and pages their token can access.

  $form['fb_stream_apps'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access Token Generator'),
    '#description' => t('Generate a token for a local application.  The new token will be placed in the form field above.'),
  );
  foreach (fb_get_all_apps() as $fb_app) {
    // Send user to this URL after token is generated.
    $redirect_uri = url($_REQUEST['q'], array(
                          'absolute' => TRUE,
                          'query' => array(
                            'client_id' => $fb_app->id,
                          ),
                        ));
    $form['fb_stream_apps'][$fb_app->id] = array(
      '#type' => 'markup',
      '#value' => l(t('post via the %title application', array(
                        '%title' => $fb_app->title,
                      )),
                    // Important to get the scope right
                    url("https://www.facebook.com/dialog/oauth", array(
                          'query' => array(
                            'scope' => 'publish_stream,offline_access,manage_pages',
                            'client_id' => $fb_app->id,
                            'redirect_uri' => $redirect_uri,
                          ),
                        )), array('html' => TRUE)),
      '#prefix' => '<p>', '#suffix' => '</p>',
    );
  }

  return system_settings_form($form);
}

function fb_stream_admin_code_to_token($code, $app_id, $redirect_uri) {
  $fb_app = fb_get_app(array('id' => $app_id));
  $path = url("https://graph.facebook.com/oauth/access_token", array(
                'query' => array(
                  'client_id' => $app_id,
                  'client_secret' => $fb_app->secret,
                  'code' => $code,
                  'redirect_uri' =>$redirect_uri,
                ),
              ));
  $http = drupal_http_request($path);

  if ($http->code == 200 && isset($http->data)) {
    $data = explode('=', $http->data);
    $token = $data[1];
    if ($token) {
      return $token;
    }
  }
}

