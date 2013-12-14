This is a utility module. The goal it to provide a simple UI to allow content
creators to append arbitrary javascript snippets to pages. It is basically an
Object Oriented wrapper around drupal_add_js that gives a simple UI for creating
and storing a snippet or a series of snippets.

*NOTICE*
By its very nature, this module is a security risk. Please note that this
module will allow users to append arbitrary unfiltered javascript and other
markup to any page. If not used with caution, this module can break or
compromise your site.


This module uses the Entity API module to store snippets as entities.
You can deploy your scripts in 2 ways:

  * WYSIWYG plugin in text editor
  * Snippet Filter, using tokens in the format of [{[js_snippet:1:Snippet%20Title]}]
  * Designate snippets to be available as blocks to be used site wide.

It can also be a useful developer tool.  You can create helper functions with
calls to frequently used combinations.

```php

// Load your snippet
$snippet = js_snippets_load(1);


$element = array(
  '#theme' => 'item_list',
  '#items' => $list_items,
);

// Fully rendered HTML with javascripts added via drupal_add_js
$element['#prefix'] = $snippet->asHtml();

// Attached to an existing element
$element = $some_unrendered_array;
$snippet->asAttached($element);

// $element will have javascripts and library dependencies added as #attached
drupal_render($element);

```