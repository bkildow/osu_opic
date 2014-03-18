<?php
/**
 * @file
 * Pushes images up to opic.
 */

/**
 * Class OsuOpicImagePusher
 */
class OsuOpicImagePusher {
  protected $nameDotNumber;
  protected $imagePath;

  /**
   * Constructor.
   *
   * @param string $name_n
   *   name.#
   * @param int $fid
   *   a drupal file id
   */
  public function __construct($name_n, $fid) {
    $this->nameDotNumber = $name_n;
    $this->imagePath = $this->getFilePath($fid);
  }

  /**
   * Pushes an image to opic.
   *
   * This will first check to see if there is already an image set for opic.
   * If so, it will fail (return false), and the image will not be pushed.
   *
   * @return bool
   *   true on success, false otherwise
   */
  public function pushImage() {

    // Short circuit if this image doesn't exist for some reason.
    if (!is_file($this->imagePath)) {
      return FALSE;
    }

    $client = new OsuOpicClient($this->nameDotNumber);

    // See if there is already an image in Opic.
    $get_http_code = $client->get();

    // If so, skip pushing this image.
    if ($get_http_code == 200) {
      return FALSE;
    }

    // POST the image to opic.
    $post_http_code = $client->post($this->imagePath);

    // If we were successful return true.
    if ($post_http_code == 201) {
      return TRUE;
    }
    else {
      return FALSE;
    }

  }

  /**
   * Gets the relative path to the file.
   *
   * @param int $fid
   *   drupal file id
   *
   * @return string
   *   the relative file path
   */
  protected function getFilePath($fid) {
    $file = file_load($fid);
    if ($file) {
      $real_path = drupal_realpath($file->uri);
      $path = str_replace($_SERVER['DOCUMENT_ROOT'] . '/', '', $real_path);
      return $path;
    }
    else {
      return '';
    }
  }
}
