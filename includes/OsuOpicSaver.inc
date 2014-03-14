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
   * @return array
   *   file array
   */
  public function saveImage() {

    // Make sure we have a name.#.
    if (!empty($this->name_n)) {
      $file = $this->saveFileData();

      // Make sure we have a file saved.
      if (!empty($file)) {
        return array(LANGUAGE_NONE => array('0' => (array) $file));
      }
    }

    return array();
  }

  /**
   * Fetches the opic image and saves the file data to the database.
   *
   * @return bool|stdClass
   */
  protected function saveFileData() {
    $filename = $this->name_n . '.jpg';
    $client = new OsuOpicClient($this->name_n);
    $response = $client->get();

    if ($response == 200) {
      return file_save_data($client->data, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);
    }
    else {
      return false;
    }
  }
}
