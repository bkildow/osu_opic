<?php
/**
 * @file
 * osu_opic.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function osu_opic_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_osu_opic'
  $field_bases['field_osu_opic'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_osu_opic',
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
  );

  return $field_bases;
}
