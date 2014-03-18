<?php
/**
 * @file
 * Pulls and saves opic images.
 */

/**
 * Class OsuOpicImagePuller
 */
class OsuOpicImagePuller {
  protected $nameDotNumber;

  /**
   * Constructor.
   *
   * @param string $name_n
   *   name.#
   */
  public function __construct($name_n) {
    $this->nameDotNumber = $name_n;
  }

  /**
   * Pulls an image from opic or false on failure.
   *
   * @return string|bool
   *   the image string or false
   */
  public function pullImage() {
    $client = new OsuOpicClient($this->nameDotNumber);
    $response = $client->get();
    if ($response == 200) {
      return $client->data;
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
   *
   * @return array
   *   file array
   */
  protected function saveFileData($file_data) {

    // Make sure we have a name.#.
    if (!empty($this->nameDotNumber)) {
      $filename = $this->nameDotNumber . '.jpg';
      $file = file_save_data($file_data, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);

      // Make sure we have a file saved.
      if (!empty($file)) {
        return array(LANGUAGE_NONE => array('0' => (array) $file));
      }
    }

    return array();
  }
}
