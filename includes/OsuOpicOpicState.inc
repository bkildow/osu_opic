<?php
/**
 * @file
 * Manages Opic states.
 */

/**
 * Class OsuOpicOpicState
 */
class OsuOpicOpicState {
  const OPIC_SYNCED = 1;
  const OPIC_LOCAL = 2;
  const OPIC_NULL = 3;

  protected $entityType;
  protected $eid;

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   the Drupal entity type
   * @param int $eid
   *   the entity id
   */
  public function __construct($entity_type, $eid) {
    $this->entityType = $entity_type;
    $this->eid = $eid;
  }

  /**
   * Returns one of the states defined as a constant in this class.
   *
   * @return int
   *   opic state
   */
  public function getState() {
    $results = $this->fetchStates();

    if ($results) {
      return $results['state'];
    }
    else {
      return self::OPIC_NULL;
    }
  }

  /**
   * Sets the opic state.
   *
   * @param int $state
   *   must be one of the integers defined as constants in this class
   */
  public function setState($state) {
    $this->validateState($state);

    db_merge('osu_opic_states')
      ->key(array('entity_type' => $this->entityType, 'eid' => $this->eid))
      ->fields(array('state' => $state, 'updated_at' => time()))
      ->execute();
  }

  /**
   * Validates the the passed state is one that this class defines.
   *
   * @param int $state
   *   opic state
   *
   * @throws Exception
   */
  protected function validateState($state) {
    $defined_states = array(
      self::OPIC_SYNCED,
      self::OPIC_LOCAL,
      self::OPIC_NULL,
    );

    if (!in_array($state, $defined_states)) {
      throw new Exception('Value must be a defined state');
    }
  }

  /**
   * Database helper method to fetch states.
   *
   * @return array
   *   array of database columns
   */
  protected function fetchStates() {
    $result = db_select('osu_opic_states', 'oos')
      ->fields('oos')
      ->condition('entity_type', $this->entityType, '=')
      ->condition('eid', $this->eid, '=')
      ->execute()
      ->fetchAssoc();

    return $result;
  }
}
