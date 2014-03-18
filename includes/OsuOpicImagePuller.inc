<?php
/**
 * @file
 * Pulls and saves opic images.
 */

/**
 * Class OsuOpicImagePuller
 */
class OsuOpicImagePuller extends OsuOpicPluginWrangler {
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
   * Pulls an image from opic or false on failure.
   *
   * @return string|bool
   *   the image string or false
   */
  public function pullImage() {
    $opicClient = new OsuOpicClient($this->name_n);
    $response = $opicClient->get();
    if ($response == 200) {
      return $opicClient->data;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Saves an image on an entity.
   *
   * @param string $image
   *   an image string, as returned by $this->pullImage()
   * @param StdClass $entity
   *   a drupal entity object
   * @param string $image_field
   *   the name of the opic image field
   */
  public function saveImage($image, $entity, $image_field) {

    $file_array = $this->saveFileData($image);

    if (!empty($file_array)) {
      $entity->{$image_field} = $file_array;
    }
  }

  /**
   * Returns a file array ready to be saved in an entity image field.
   *
   * @param string $file_data
   *   the image data as returned by an opic request
   * @return array
   *   file array
   */
  protected function saveFileData($file_data) {

    // Make sure we have a name.#.
    if (!empty($this->name_n)) {
      $filename = $this->name_n . '.jpg';
      $file = file_save_data($file_data, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);

      // Make sure we have a file saved.
      if (!empty($file)) {
        return array(LANGUAGE_NONE => array('0' => (array) $file));
      }
    }

    return array();
  }
}
