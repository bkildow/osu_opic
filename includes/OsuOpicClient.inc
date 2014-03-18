<?php
/**
 * @file
 * Manages HTTP requests to opic.
 */

/**
 * Class OsuOpicClient
 */
class OsuOpicClient {

  protected $nameDotNumber;
  protected $baseUrl = 'https://opic.osu.edu';
  public $data;
  public $code;

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
   * Resets the request properties.
   */
  protected function resetRequestData() {
    $this->code = '';
    $this->data = '';
  }

  /**
   * Fetches an opic image.
   *
   * If the person we are requesting is opted out, filtered out for FERPA flag,
   * have no image or there is no such name.n, this will return 302.
   * If there is a 200 response, the image can be found at $this->data.
   *
   * @return int
   *   http response code
   */
  public function get() {
    $this->resetRequestData();

    $url = $this->baseUrl . '/' . $this->nameDotNumber . '?aspect=p&width=500';
    $response = drupal_http_request($url);

    // If we were redirected, it means the person doesn't have a pic.
    if (!empty($response->redirect_code)) {
      $this->code = $response->redirect_code;
    }
    else {
      $this->code = $response->code;
      $this->data = $response->data;
    }

    return $this->code;
  }

  /**
   * POSTS an image to opic. Will return 201 on success.
   *
   * @param string $image_path
   *   the path to the image
   *
   * @return int
   *   the http status code
   */
  public function post($image_path) {
    $this->resetRequestData();

    $url = $this->baseUrl . '/api/avatars';

    $opic_api_key = variable_get('opic_api_key');
    $api_key = 'X-API-KEY: ' . $opic_api_key;

    $header = array('Content-Type: multipart/form-data', $api_key);
    $fields = array(
      'avatar' => '@' . $image_path,
      'name_n' => $this->nameDotNumber,
    );

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $fields);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $fields);
    curl_setopt($ch, CURLOPT_SSLVERSION, 3);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
    $result = curl_exec($ch);
    $this->code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);

    return $this->code;
  }

}
