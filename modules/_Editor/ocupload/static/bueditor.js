BUE.postprocess.ocupload = function(E, $) {
  E.showFileSelectionDialog = function () {
    alert(Drupal.t('Error while module initialize.'));
  }
  
  for (var i = 0; i < E.tpl.buttons.length; i++) {
    if (E.tpl.buttons[i][1] == 'js: E.showFileSelectionDialog();') {
      var $button = $('#bue-' + E.index + '-button-' + i);
      
      $button.load(function() {
        var buttonWidth = $button.width();
        var buttonHeight = $button.height();

        if (Drupal.settings.ocupload.allowedExt == '*.;') {
          $button.remove();
          return;
        }

        var $wrapper = $('<span class="ocupload-button-wrapper bue-button"></span>').css({
          width: buttonWidth,
          height: buttonHeight
        });

        if ($button.hasClass('bue-sprite-button')) {
          $wrapper.css({
            'background-image'    : $button.css('background-image'),
            'background-position' : $button.css('background-position')
          })
        }
        else {
          $wrapper.css('background', 'url(' + $button.attr('src') + ') no-repeat center center')
        }

        $button.wrap($wrapper);
        $button.closest('.bue-ui').append('<span id="ocupload-progress-' + E.index + '" class="ocupload-progress"></span>');
        
        var swfu = new SWFUpload({
          flash_url              : Drupal.settings.basePath + 'sites/all/libraries/swfupload/Flash/swfupload.swf',
          upload_url             : Drupal.settings.basePath + 'ocupload/upload',
          button_placeholder_id  : $button.attr('id'),
          file_size_limit        : Drupal.settings.ocupload.sizeLimit + ' B',
          file_types             : Drupal.settings.ocupload.allowedExt,
          file_types_description : Drupal.t('Files'),
          file_upload_limit      : 0,
          prevent_swf_caching    : false,
          button_window_mode     : SWFUpload.WINDOW_MODE.TRANSPARENT,
          button_width           : buttonWidth,
          button_height          : buttonHeight,
          button_cursor          : SWFUpload.CURSOR.HAND,
          button_text            : $button.attr('type') == 'button' ? $button.val() : '',
          file_post_name         : 'files[file]',
          post_params            : {'phpsessid':Drupal.settings.ocupload.phpsessid},
          // after files select
          file_dialog_complete_handler: function() {
            swfu.addPostParam('selectedText', BUE.active.getSelection());
            swfu.addPostParam('formId', $('textarea[name="' + E.textArea.name + '"]').closest('form').find('input[name="form_id"]').val());
            swfu.addPostParam('fieldName', E.textArea.name);
            this.startUpload();
          },
          // start upload one file in queue
          upload_start_handler: function(file) {
            $('#ocupload-progress-' + E.index).html('Uploading ' + file.name);
          },
          // after upload one file
          upload_success_handler: function(file, serverData) {
            if (serverData.substring(0, 1) != '{') {
              return alert(Drupal.t('Server response came not in JSON format') + ': "' + serverData + '"');
            }
            response = $.parseJSON(serverData);
            if (response.status) {
              BUE.active.replaceSelection(response.data + "\n", 'end');
            }
            else {
              alert(response.data);
            }
            BUE.active.focus();
          },
          // after upload all files
          queue_complete_handler: function() {
            $('#ocupload-progress-' + E.index).html('');
          }
        });
        
        $button.parent().mousedown(function(event){ event.stopPropagation(); });
      });
      
      if ($button.attr('type') != 'image') {
        $button.load();
      }
      
      break;
    }
  }
};