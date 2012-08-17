Install module:
---------------
1. Extract module archive in "sites/all/modules".

2. Enable module "One Click Upload".

3. Download SWFUpload v2.2.0.1 from http://code.google.com/p/swfupload/downloads/list, extract archive in "sites/all/libraries" and rename dir "SWFUpload v2.2.0.1 Core" to "swfupload". After this file "swfupload.js" must be available on path "sites/all/libraries/swfupload/swfupload.js". Drush users can use the command "drush ocupload-dl-library".


Integrate to BUEditor:
----------------------
1. Open BUEditor config page "admin/config/content/bueditor".

2. Click "Edit" link for your use editor.

3. Add new button with code: "js: E.showFileSelectionDialog();" (do not change this code!).

4. Click "Save configuration".


Integrate to standalone CKEditor 7.x-1.4+ (not Wysiwyg module):
---------------------------------------------------------------
1. Open CKEditor config page "admin/config/content/ckeditor".

2. Click "edit" link for your use editor.

3. Open section "Editor appearance", go to "Toolbar" field and drag&drop "One Click Upload" icon from "All buttons" to "Used buttons".

4. Below, in "Plugins" field, choose checkbox "One Click Upload".

5. Click "Save".

6. Clear your browser cache.


Integrate to CKEditor with Wysiwyg:
-----------------------------------
1. Open Wysiwyg cofig page admin/config/content/wysiwyg.

2. Click  Edit link for your use editor.

3. Open fieldset Buttons and plugins and mark checkbox One Click UPload.

4. Click Save.
