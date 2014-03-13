<?php

/**
 * @file
 * handles interface to opic.osu.edu
 */

/**
 * Class OsuOpicData
 */
class OsuOpicData extends OsuOpicPluginWrangler {

  protected $baseUrl = 'https://opic.osu.edu';

  public function getImage($name_dot_number) {
    $filename = $name_dot_number . '.jpg';
    $url = $this->baseUrl . '/' . $name_dot_number . '?aspect=p&width=500';
    $image = file_get_contents($url);
    return file_save_data($image, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);
  }
}
