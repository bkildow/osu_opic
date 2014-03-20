<?php
/**
 * @file
 * Manages tasks for queueing.
 */

/**
 * Class OsuOpicQueueWorker
 */
class OsuOpicQueueWorker {

  /**
   * Gets items that need to be updated.
   *
   * @return array
   *   array of objects containing eid and entity_type properties
   */
  public function getItems() {
    $field_info = field_info_field_map();
    $items = array();

    foreach ($field_info[OSU_OPIC_IMAGE_FIELD]['bundles'] as $entity_type => $bundles) {

      $entity_info = entity_get_info($entity_type);
      $eid_identifier = $entity_info['entity keys']['id'];
      $base_table = $entity_info['base table'];

      $query = $this->generateBaseQuery($base_table, $eid_identifier, $entity_type);

      if ($entity_type == 'node' && !empty($bundles)) {
        $query->condition('type', $bundles, 'IN');
      }

      $results = $query->execute()->fetchAll();
      $parsed_results = $this->parseItemResults($results, $entity_type);
      $items = array_merge($parsed_results, $items);
    }

    return $items;
  }

  /**
   * Update the entity from the data object passed.
   *
   * @param StdClass $data
   *   an object with eid and entity_type properties
   */
  public function updateEntity($data) {

    // Make sure we have valid data.
    if (intval($data->eid) == 0 || !isset($data->entity_type)) {
      watchdog('osu_opic', 'Updating failed for entity of type ' . $data->entity_type . ' with id: ' . $data->eid . ', due to invalid being passed.');
      return;
    }

    // Load the entity we want to save.
    $entity = entity_load_single($data->entity_type, $data->eid);

    // If it was loaded, save away!
    if (!empty($entity)) {
      entity_save($data->entity_type, $entity);
    }
    else {
      watchdog('osu_opic', $data->entity_type . ' Entity with id: ' . $data->eid . ' failed updating');
    }
  }

  /**
   * Processes the queue.
   *
   * This is mostly based on the implementation in drupal_cron_run(), but
   * with logging added to show how many items were processed.
   */
  public function processQueue() {
    $queue_name = 'osu_opic';
    $queues = osu_opic_cron_queue_info();
    $info = $queues[$queue_name];

    $function = $info['worker callback'];
    $end = time() + (isset($info['time']) ? $info['time'] : 15);
    $queue = DrupalQueue::get($queue_name);
    $start_items = $queue->numberOfItems();

    while (time() < $end && ($item = $queue->claimItem())) {
      $function($item->data);
      $queue->deleteItem($item);
    }

    $end_items = $queue->numberOfItems();
    $items_processed = $start_items - $end_items;

    watchdog('osu_opic', $items_processed . ' items processed.');
  }

  /**
   * Gets excluded entities.
   *
   * To be excluded, the updated_at timestamp must be less than a day old,
   * or the opic state must be set to LOCAL.
   *
   * @param string $base_table
   *   the entity table to operate on
   * @param string $eid_identifier
   *   the id name (nid, uid, etc.).
   * @param string $entity_type
   *   the drupal entity type
   *
   * @return array
   *   an indexed array of entity ids
   */
  protected function getExcludedEntities($base_table, $eid_identifier, $entity_type) {
    $prior_timestamp = strtotime('-1 day');

    $query = db_select($base_table, 'e');
    $query->addField('e', $eid_identifier, 'eid');
    $query->join('osu_opic_states', 'oos', 'e.' . $eid_identifier . ' = oos.eid');

    $db_or = db_or();
    $db_or->condition('oos.state', OsuOpicOpicState::OPIC_LOCAL, '=');
    $db_or->condition('oos.updated_at', $prior_timestamp, '>');
    $query->condition($db_or);
    $query->condition('oos.entity_type', $entity_type, '=');

    return $query->execute()->fetchCol();
  }

  /**
   * Adds entity_type to the results array objects.
   *
   * @param array $results
   *   results array
   * @param string $entity_type
   *   drupal entity type
   *
   * @return array
   *   array of objects with eid and entity_type properties.
   */
  protected function parseItemResults($results, $entity_type) {
    $return = array();
    foreach ($results as $result) {
      $result->entity_type = $entity_type;
      $return[] = $result;
    }
    return $return;
  }

  /**
   * Generates the base query for getting entities needed to be updated.
   *
   * @param string $base_table
   *   the entity table to operate on
   * @param string $eid_identifier
   *   the id name (nid, uid, etc.).
   * @param string $entity_type
   *   the drupal entity type
   *
   * @return SelectQuery
   *   query object
   */
  protected function generateBaseQuery($base_table, $eid_identifier, $entity_type) {
    $excluded_entities = $this->getExcludedEntities($base_table, $eid_identifier, $entity_type);

    $query = db_select($base_table, 'e');
    $query->addField('e', $eid_identifier, 'eid');

    if (!empty($excluded_entities)) {
      $query->condition($eid_identifier, $excluded_entities, 'NOT IN');
    }

    return $query;
  }
}
