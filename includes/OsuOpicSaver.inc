<?php

/**
 * @file
 * Handles saving data for the opic image field.
 */

/**
 * Class OsuOpicSaver
 */
class OsuOpicSaver extends OsuOpicPluginWrangler {

  public $name_n;

  protected $entity_type;
  protected $entity;
  protected $image_field;

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   the type of Drupal entity
   * @param StdClass $entity
   *   a Drupal entity object
   * @param string $image_field
   *   the image field which has opic settings attached
   */
  public function __construct($entity_type, $entity, $image_field) {
    $this->entity_type = $entity_type;
    $this->entity = $entity;
    $this->$image_field = $image_field;
    $this->name_n = $this->getNameDotNumber($entity_type, $entity, $image_field);
  }

  /**
   * Returns a file array ready to be saved in an entity image field.
   *
   * @param string $image
   *   the image data as returned by an opic request
   * @return array
   *   file array
   */
  public function saveImage($image) {

    // Make sure we have a name.#.
    if (!empty($this->name_n)) {
      $filename = $this->name_n . '.jpg';
      $file = file_save_data($image, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);

      // Make sure we have a file saved.
      if (!empty($file)) {
        return array(LANGUAGE_NONE => array('0' => (array) $file));
      }
    }

    return array();
  }

}
