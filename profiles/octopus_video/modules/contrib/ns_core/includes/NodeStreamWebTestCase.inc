<?php
/**
 * @file
 * Basic test cases for NodeStream.
 */

/**
 * Base Web test case.
 */
class NodeStreamWebTestCase extends DrupalWebTestCase {
  protected $testFiles = array();

  /**
   * This setUp properly works without warnings when using another installation
   * profile than testing.
   */
  protected function setUp() {
    $profile = drupal_get_profile();
    // Use the NodeStream profile if that's what's used currently.
    if ($profile == 'nodestream') {
      $this->profile = 'nodestream';
    }
    $modules = func_get_args();
    parent::setUp($modules);
  }

  /**
   * Clear the test files array between tests.
   */
  function tearDown() {
    $this->testFiles = array();
    parent::tearDown();
  }

  /**
   * This is a hack to be able to call any method within the test case class.
   * This might seems like something horrible, but it actually let's us separate
   * functionality in a a sane way.
   * @param $method
   *   The method to call
   * ...
   * the arguments to provide.
   */
  public function callTestCaseFunction($method, $args) {
    return call_user_func_array(array($this, $method), $args);
  }

  /**
   * A sane version of DrupalWebTestCase::drupalLogin().
   */
  function drupalLogin(stdClass $user) {
    if ($this->loggedInUser) {
      $this->drupalLogout();
    }

    $edit = array(
      'name' => $user->name,
      'pass' => $user->pass_raw
    );
    $this->drupalPost('user', $edit, t('Log in'));
    // The user should be redirected to it's profile page.
    $pass = $this->assertTrue($this->getUrl(), 'user/' . $user->uid, t('User %name successfully logged in.', array('%name' => $user->name)), t('User login'));
    if ($pass) {
      $this->loggedInUser = $user;
    }
  }

  /**
   * Grant a series of roles to particular user.
   * @param stdClass $user a user object
   * @param array roles an array of role names.
   */
  function grantRoles($user, array $roles) {
    $roles_by_id = user_roles();
    foreach ($roles as $role) {
      $role_id = array_search($role, $roles_by_id);
      $user->roles[$role_id] = $role;
    }
    user_save($user);
  }

  /**
   * Get a nice lorem ipsum string.
   * @return string
   *   A string of lorem ipsum text.
   */
  protected function loremIpsum() {
    return 'Cupcake ipsum dolor sit. Amet icing bear claw gingerbread cake danish. Fruitcake cookie bear claw danish. Pie topping gummi bears candy biscuit gingerbread carrot cake topping. Jelly-o pastry lemon drops. Marshmallow jelly-o chocolate bar sugar plum bonbon halvah fruitcake. Liquorice dragée croissant chocolate tart tiramisu lollipop. Soufflé pudding liquorice lemon drops. Brownie gummi bears tart oat cake bonbon tiramisu. Faworki toffee liquorice jelly-o. Marzipan tiramisu cake jelly-o cheesecake. Cake pastry brownie powder chupa chups. Pie dessert cake pie ice cream sugar plum marzipan icing.';
  }

  /**
   * Assert that a a particular format exists
   * in a select element of input formats.
   * @param $field a form field.
   * @param $format a format to check for.
   * @param $message the message to display.
   */
  function assertFormat($field, $format, $message = NULL) {
    if (empty($message)) {
      $message = "The format $format exists.";
    }
    $this->assertFormatHelper($field, $format, $message, TRUE);
  }

  /**
   * Assert that a a particular format isn't available
   * in a select element of input formats.
   * @param $field a form field.
   * @param $format a format to check for.
   * @param $message the message to display.
   */

  function assertNoFormat($field, $format, $message = NULL) {
    if (empty($message)) {
      $message = "The format $format doesn't exist.";
    }
    $this->assertFormatHelper($field, $format, $message, FALSE);
  }

  /**
   * Helper for assertFormat and assertNoFormat.
   */
  protected function assertFormatHelper($field, $format, $message, $exists) {
    $select = $this->xpath("//select[@name='$field']");
    foreach ($select[0]->option as $option) {
      if ($option['value'] == $format) {
        $exists ? $this->pass($message) : $this->fail($message);
        return;
      }
    }
    $exists ? $this->fail($message) : $this->pass($message);
  }

  /**
   * Create a test file.
   */
  protected function createTestFile($type = 'image') {
    if (!isset($this->testFiles[$type])) {
      $this->testFiles[$type] = current($this->drupalGetTestFiles($type));
      file_save($this->testFiles[$type]);
    }
    return $this->testFiles[$type];
  }
}


/**
 * This class can be extended by various classes that you want to use
 * as objects in your code.
 */
abstract class TestHelper {
  protected $testCase;
  function __construct($test_case) {
    $this->testCase = $test_case;
  }

  function __call($name, $arguments) {
    return $this->testCase->callTestCaseFunction($name, $arguments);
  }
}