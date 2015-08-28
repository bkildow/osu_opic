# OSU Opic Image field

This module provides an image field which upon saving will pull a profile
picture from the opic.osu.edu image service. This can also be overridden
locally, as well as potentially pushed up to opic (see behavior section below).

## Configuration

First, make sure you have your opic API key in settings.php. You should add
it like so:

`$conf['opic_api_key'] = 'abc123';`

If you are using an image field that is named something other than 
`field_osu_opic`, you can set the field machine name via drush. E.g.
`drush vset osu_opic_image_field <FIELD_NAME>`

Next, when you add this field to a content type, make sure you configure the
opic settings on the field. This setting lets you choose an Opic Field plugin,
which is basically tells the opic field how to get a name.#. Currently,
osu_opic comes with a text field plugin which knows how to read name.#'s from
regular text fields. This is implemented as a custom CTools plugin, and can
easily be extended to add other field types which may include a name.# (like
an entityreference field for example).

## Behavior

When saving an entity with this field attached, one of these behaviors are
possible:

### Image gets pushed to opic

If an image is new, the image will be pushed up to opic.
This will fail if there is already an image set in opic, or if the person has
opted-out. In which case the image will be marked as LOCAL. If successful,
the image will be marked as SYNCED and will continue to update the image in the
future if a person changes their opic image on opic.osu.edu.

### Image gets pulled from opic

If was no image added, one will be pulled from opic, and it will be set
as SYNCED. This will continue to update the image in the future if a person
changes their opic image on opic.osu.edu.

### Image gets set as LOCAL

If an image is uploaded overriding an existing image, it will be set as LOCAL
and not interact with opic. If an image is LOCAL and is deleted, it will
go back to being SYNCED, and will be pulled from opic.


## For developers:

### States

To determine which behavior to use, there are several file states and opic
states that this module keeps track of, such as if the file is newly uploaded,
deleted, changed, etc. The following file states as defined by the module are:

 * IMAGE_EMPTY_UNCHANGED - No image has been uploaded
 * IMAGE_EXISTS_UNCHANGED - The image was not changed
 * IMAGE_NEW - A new image has been uploaded
 * IMAGE_UPDATED - A new image was uploaded over a previous image
 * IMAGE_DELETED - An image was deleted
 * IMAGE_NULL - No state has been set

 Also the module defines states to know how to interact with opic:

 * OPIC_SYNCED - We should interact with the opic image
 * OPIC_LOCAL - We should not interact with opic, and just use the local image
 * OPIC_NULL - No state has been set

See the various state classes (OsuOpicFileState, OsuOpicOpicState,
OsuOpicStateManager) to see how the logic flow works.

### CTools plugin

To create your own opic CTools plugin to provide a name.#, you must first
include hook_ctools_plugin_directory to let CTools know where your plugin can
be found.

```
/**
 * Implements hook_ctools_plugin_directory().
 */
function MY_MODULE_ctools_plugin_directory($owner, $plugin_type) {
  if ($owner == 'osu_opic') {
    return "plugins/$plugin_type";
  }
}
```

Next, create the folder structure: plugins/opic, an put your plugin here.
To see how to create the plugin, look at plugins/opic/textfield.inc in this
module as an example.

After this, clear your cache, and you should now see your plugin showing up
in the field settings for the opic field.
