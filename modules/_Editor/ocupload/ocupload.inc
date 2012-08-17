<?php
/**
 * @file
 * Service functions
 */

/**
 * Admin page with templates list
 */
function ocupload_config_page() {
  $templates = ocupload_templates();
  $table_header = array(t('Mask'), t('Operations'));
  $table_data = array();
  $output = '';

  foreach ($templates as $template) {
    $actions = array(
      l(t('edit'), 'admin/config/content/ocupload/edit/' . $template->tid),
      l(t('delete'), 'admin/config/content/ocupload/delete/' . $template->tid),
    );
    $table_data[] = array($template->mask, implode(' | ', $actions));
  }

  $output .= '<h3>' . t('Templates') . '</h3>';
  $output .= theme('table', array('header' => $table_header, 'rows' => $table_data));

  $output .= '<br /><h3>' . t('Options') . '</h3>';
  $output .= render(drupal_get_form('ocupload_form_settings'));

  return $output;
}

/**
 * Form module settings
 */
function ocupload_form_settings($form, &$form_state) {
  $form['ocupload_delete_unused_files'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete unused files'),
    '#description' => t('To save space uploaded files that are not used in text will be deleted'),
    '#default_value' => variable_get('ocupload_delete_unused_files', 1),
  );
  return system_settings_form($form);
}

/**
 * Form add/edit template
 */
function ocupload_form_template($form, &$form_state, $template = NULL) {
  $public_path = variable_get('file_public_path', conf_path() . '/files');

  if (!$template) {
    $template = (object)array(
      'tid' => 0,
      'mask' => '',
      'path' => '',
      'template' => '<a href="!filepath">!filename</a>',
      'template_select' => '<a href="!filepath">!text</a>',
      'image_style' => '',
      'link_to_original' => 0,
      'link_only_big' => 0,
      'rename_file' => 1,
    );
    drupal_set_title(t('Add template'));
  }

  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $template->tid,
  );

  $form['mask'] = array(
    '#type' => 'textfield',
    '#title' => t('File extensions'),
    '#description' => t('Comma separated list of file extensions which should be handled within this template. Example: <code>jpg,gif,png</code>'),
    '#default_value' => $template->mask,
    '#size' => 40,
    '#required' => TRUE,
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload path'),
    '#description' => t('Dir name, relative <code>!path</code>, with will upload files. Without the initial and trailing slashes.', array('!path' => $public_path)),
    '#default_value' => $template->path,
    '#field_prefix' => $public_path . '/',
    '#size' => 22,
    '#required' => TRUE,
  );

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  
    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('user'),
    );
  }

  $form['template'] = array(
    '#type' => 'textfield',
    '#title' => t('Template to insert into editor'),
    '#default_value' => $template->template,
    '#maxlength' => NULL,
    '#size' => 100,
    '#required' => TRUE,
  );

  $form['template_select'] = array(
    '#type' => 'textfield',
    '#title' => t('Template to replace selected text'),
    '#default_value' => $template->template_select,
    '#maxlength' => NULL,
    '#size' => 100,
    '#required' => TRUE,
  );

  $form['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['help']['text'] = array(
    '#markup' => '
      !filepath     — ' . t('full path to file') . '<br />
      !filename     — ' . t('file name after upload') . '<br />
      !originalname — ' . t('original file name') . '<br />
      !filesize     — ' . t('file size') . '<br />
      !text         — ' . t('selected text') . '<br />
    ',
  );

  if (module_exists('image')) {
    $form['image_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options for images'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['image_settings']['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Use image style'),
      '#options' => _ocupload_get_image_styles(),
      '#default_value' => $template->image_style,
    );

    $form['image_settings']['link_to_original'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add link to original image'),
      '#default_value' => $template->link_to_original,
      '#states' => array(
        'invisible' => array(
          'select[name="image_style"]' => array('value' => ''),
        )
      ),
    );

    $form['image_settings']['link_only_big'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add link only for those images which size exceeds the size, specified in the style'),
      '#default_value' => $template->link_only_big,
      '#states' => array(
        'visible' => array(
          'input[name="link_to_original"]' => array('checked' => TRUE),
        )
      ),
    );
    
    if (module_exists('colorbox')) {
      $form['image_settings']['show_in_colorbox'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show original image in Colorbox'),
        '#default_value' => $template->show_in_colorbox,
        '#states' => array(
          'visible' => array(
            'input[name="link_to_original"]' => array('checked' => TRUE),
          )
        ),
      );
    }
  }

  $form['rename_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rename uploaded file'),
    '#description' => t('Checked, if you want to rename uploded files according to current date/time.'),
    '#default_value' => $template->rename_file,
  );

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Checked roles which can upload files of this type.'),
    '#options' => array_map('check_plain', user_roles()),
    '#default_value' => array_keys(user_roles(FALSE, 'upload files use template ' . $template->tid)),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $template->tid ? t('Save template') : t('Add template'),
  );

  return $form;
}

/**
 * Form submit
 */
function ocupload_form_template_submit($form, &$form_state) {
  $template = array(
    'tid'             => $form_state['values']['tid'],
    'mask'            => $form_state['values']['mask'],
    'path'            => $form_state['values']['path'],
    'template'        => $form_state['values']['template'],
    'template_select' => $form_state['values']['template_select'],
    'rename_file'     => $form_state['values']['rename_file'],
  );
  
  if (module_exists('image')) {
    $template += array(
      'image_style'      => $form_state['values']['image_style'],
      'link_to_original' => $form_state['values']['link_to_original'],
      'link_only_big'    => $form_state['values']['link_only_big'],
    );
    if (module_exists('colorbox')) {
      $template += array(
        'show_in_colorbox' => $form_state['values']['show_in_colorbox'],
      );
    }
  }
  
  ocupload_template_save($template);
  
  foreach ($form_state['values']['roles'] as $rid => $enabled) {
    user_role_change_permissions($rid, array('upload files use template ' . $template['tid'] => $enabled));
  }
  
  drupal_set_message($form_state['values']['tid'] ? t('Template saved') : t('Template added'));
  $form_state['redirect'] = 'admin/config/content/ocupload';
}

/**
 * Confirm form delete teamplate
 */
function ocupload_delete_confirm($form, &$form_state, $template) {
  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $template->tid,
  );

  return confirm_form(
    $form,
    t('Do you really want to delete template') . ' ' . $template->tid . '?',
    'admin/config/content/ocupload'
  );
}

/**
 * Confirm form submit
 */
function ocupload_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    ocupload_template_delete($form_state['values']['tid']);
    drupal_set_message(t('Deleted'));
  }

  drupal_goto('admin/config/content/ocupload');
}

/**
 * Upload file
 */
function ocupload_upload() {
  global $user, $base_url;

  _ocupload_auth();

  $templates = ocupload_templates();
  $extension_uploaded_file = _ocupload_get_file_extension($_FILES['files']['name']['file']);
  $selected_text = !empty($_POST['selectedText']) ? $_POST['selectedText'] : '';

  foreach ($templates as $template) {
    if (in_array($extension_uploaded_file, explode(',', $template->mask)) && user_access('upload files use template ' . $template->tid)) {
      $destination = 'public://' . token_replace($template->path, array('user' => $user));
      file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
      $file = file_save_upload('file', array('file_validate_extensions' => array(str_replace(',', ' ', $template->mask))), $destination);

      if ($file) {
        if (!variable_get('ocupload_delete_unused_files', 1)) {
          $file->status = FILE_STATUS_PERMANENT;
          $file = file_save($file);
        }

        if ($template->rename_file) {   
          $file = file_move($file, $destination . '/' . date('YdmHis') . '.' . $extension_uploaded_file);
        }

        $cur_template = $selected_text ? $template->template_select : $template->template;
        $filepath = $file->uri;

        // If upload image and template image style not empty
        if ($selected_text == '' && _ocupload_is_image($file->filename) && $template->image_style && module_exists('image')) {
          $filepath = image_style_path($template->image_style, $file->uri);

          if ($template->link_to_original) {
            $wrap_template = TRUE;
            if ($template->link_only_big) {
              $image_style = image_style_load($template->image_style);
              $image_style_effects = current($image_style['effects']);
              $image_style_width = isset($image_style_effects['data']['width']) ? $image_style_effects['data']['width'] : 0;
              $image_style_height = isset($image_style_effects['data']['height']) ? $image_style_effects['data']['height'] : 0;
              $image_info = image_get_info($file->uri);

              if (!(($image_style_width > 0 && $image_info['width'] > $image_style_width) || ($image_style_height > 0 && $image_info['height'] > $image_style_height))) {
                $wrap_template = FALSE;
              }
            }
            if ($wrap_template) {
              $attributes = array('href' => _ocupload_get_local_url($file->uri), 'target' => '_blank');
              if ($template->show_in_colorbox) {
                $attributes['class'] = 'colorbox-load';
              }
              $cur_template = '<a' . drupal_attributes($attributes) . '>' . $cur_template . '</a>';
            }
          }
        }

        $data = strtr($cur_template, array(
          '!filepath'     => _ocupload_get_local_url($filepath),
          '!filename'     => $file->filename,
          '!originalname' => basename($file->destination),
          '!filesize'     => format_size($file->destination),
          '!text'         => $selected_text,
        ));

        if (variable_get('ocupload_delete_unused_files', 1)) {
          $cid = 'ocupload:' . $_POST['formId'] . ':' . $user->uid;
          if ($cache = cache_get($cid)) {
            $files = $cache->data;
          }
          $files[$_POST['fieldName']][$file->fid] = $file->filename;
          cache_set($cid, $files, 'cache', REQUEST_TIME + DRUPAL_MAXIMUM_TEMP_FILE_AGE);
        }

        return _ocupload_answer(TRUE, $data);
      }
      // If upload error
      else {
        $errors = drupal_get_messages(null, TRUE);
        $data = array();
        foreach ($errors as $errorPerType) {
          $data[] = strip_tags(implode("\n\n", $errorPerType));
        }
        return _ocupload_answer(FALSE, implode("\n\n", $data));
      }

      break;
    }
  }

  return _ocupload_answer(FALSE, t('This file type can not be upload'));
}

/**
 * Add/save template
 */
function ocupload_template_save(&$template, $multi = FALSE) {
  if ($multi) {
    return array_map('ocupload_template_save', $template);
  }

  return drupal_write_record('ocupload_templates', $template, !empty($template['tid']) ? 'tid' : array());
}

/**
 * Delete template
 */
function ocupload_template_delete($tid) {
  db_delete('ocupload_templates')->condition('tid', $tid)->execute();
}

/**
 * Authorize by $_POST data
 */
function _ocupload_auth() {
  $phpsessid = isset($_POST['phpsessid']) ? $_POST['phpsessid'] : FALSE;
  if ($phpsessid && session_id() != $phpsessid) {
    @session_destroy();
    session_id($phpsessid);
    $_COOKIE[session_name()] = $phpsessid;
    drupal_session_started(FALSE);
    drupal_session_initialize();
  }
}

/**
 * Return file extension
 */
function _ocupload_get_file_extension($filename) {
  if (preg_match('#\.([a-zA-Z0-9]+)$#', $filename, $matches)) {
    return strtolower($matches[1]);
  }
}

/**
 * Return local url by uri
 * Example: "_ocupload_get_local_url('public://images/logo.jpg')" return "/sites/default/files/images/logo.jpg"
 */
function _ocupload_get_local_url($uri) {
  return drupal_substr(file_create_url($uri), drupal_strlen($GLOBALS['base_root']));
}

/**
 * Return image styles for use in #options
 */
function _ocupload_get_image_styles() {
  $image_styles = image_styles();
  $image_style_options = array('' => t('none'));
  foreach ($image_styles as $name => $image_style) {
    $image_style_options[$name] = $image_style['name'];
  }
  return $image_style_options;
}

/**
 * Return TRUE if file extension equal jpg,gif,png,bmp
 */
function _ocupload_is_image($filename) {
  $extension = _ocupload_get_file_extension($filename);
  return in_array($extension, array('jpg', 'jpeg', 'png', 'gif', 'bmp'));
}

/**
 * Return answer in json format
 */
function _ocupload_answer($status, $data) {
  drupal_json_output(array('status' => $status, 'data' => $data));
}
