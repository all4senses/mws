<?php

/**
 * Implementation of hook_drush_command().
 */
function ocupload_drush_command() {
  $items['ocupload-dl-library'] = array(
    'description' => dt('Download and install SWFUpload library'),
  );
  return $items;
}

/**
 * Command callback.
 */
function drush_ocupload_dl_library() {
  $olddir = getcwd();
  $path = $olddir . '/sites/all/libraries';
  $url = 'http://swfupload.googlecode.com/files/SWFUpload v2.2.0.1 Core.zip';
  $filename = basename($url);
  
  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  
  drush_op('chdir', drush_tempdir());
  
  // Download the zip archive
  if (!drush_shell_exec('wget %s', $url)) {
    drush_shell_exec('curl -O %s', $url);
  }
  if (!file_exists($filename)) {
    drush_op('chdir', $olddir);
    return drush_set_error(dt('Unable to download @url', array('@url' => $url)));
  }
  
  // Decompress the zip archive
  drush_shell_exec("tar -xf %s -C %s", $filename, $path);
  // Remove the zip archive
  drush_op('unlink', $filename);
  // Rename extracted dir
  drush_move_dir($path . '/SWFUpload v2.2.0.1 Core', $path . '/swfupload');
  
  drush_op('chdir', $olddir);
  drush_print('SWFUpload downloaded and installed.');
}
