<?php

class OsuOpic_Data extends OsuOpic_PluginWrangler{

  private $base_url = 'https://opic.osu.edu';

  function getImage($name_dot_number) {
    $filename = $name_dot_number.'.jpg';
    $url = $this->base_url.'/'.$name_dot_number.'?aspect=p&width=500';
    $image = file_get_contents($url);
    return file_save_data($image, 'public://opic/' . $filename, FILE_EXISTS_REPLACE);
  }
}

