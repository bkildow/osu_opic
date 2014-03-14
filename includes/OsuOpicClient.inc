<?php

class OsuOpicClient {

  protected $name_n;
  protected $base_url = 'https://opic.osu.edu';
  public $data;
  public $code;

  public function __construct($name_n) {
    $this->name_n = $name_n;
  }

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
   * @return bool
   *   returns true upon success or false otherwise
   */
  public function get() {
    $this->resetRequestData();

    $url = $this->base_url . '/' . $this->name_n . '?aspect=p&width=500';
    $response = drupal_http_request($url);

    // If we were redirected, it means the person doesn't have a pic.
    if(!empty($response->redirect_code)) {
      $this->code = $response->redirect_code;
      return false;
    }
    else {
      $this->code = $response->code;
      $this->data = $response->data;
      return true;
    }
  }

  public function post() {

  }

}
