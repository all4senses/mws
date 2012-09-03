<?php if (!$page): ?>
  <article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
<?php endif; ?>

  <?php if ($user_picture || $display_submitted || !$page): ?>
    <?php if (!$page): ?>
      <header>
	<?php endif; ?>

      <?php print $user_picture; ?>
  
      <?php print render($title_prefix); ?>
      <?php if (!$page): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
  
      <?php if ($display_submitted): ?>
        <span class="submitted"><?php print $submitted; ?></span>
      <?php endif; ?>

    <?php if (!$page): ?>
      </header>
	<?php endif; ?>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // Hide comments, tags, and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      print render($content);
    ?>
  </div>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
      <?php print render($content['field_tags']); ?>
      <?php //print render($content['links']); ?>
    </footer>
  <?php endif; ?>

    
<?php echo 'Test Dept'; 
    
    
dpm($node);

//$term_children = taxonomy_get_children($node->field_category['und'][0]['taxonomy_term']->tid, $node->field_category['und'][0]['taxonomy_term']->vid);
$term_children = taxonomy_get_children($node->field_category['und'][0]['tid']);//, $node->field_category['und'][0]['taxonomy_term']->vid);


$tids = NULL;
foreach($term_children as $term_child) {
  $tids[$term_child->tid] = $term_child->tid;
}

if ($tids) {
  $child = NULL;
  foreach ($tids as $tid) {
    if($nodes = taxonomy_select_nodes($tid)) {
      $child = node_load($nodes[0]);
      break;
    }
  }
  
  if (!$child || $child->type == 'dept') {
    $display = 'block_depts';
  }
  else {
    $display = 'block_products';
  }
 
  $view = views_get_view('catalog_node_siblings');
  $options = array(
    'id' => 'tid',
    'value' => $tids, 
    'type' => 'select',
    'vid' =>  'catalog',
    'hierarchy' => 1,
    'reduce_duplicates' => 1,
    'group' => 0,
  );
  $view->add_item($display, 'filter', 'taxonomy_index', 'tid', $options);

  
  
  
  
  
  
  $relationship = NULL;
  $order = (@$_GET['order'] ? $_GET['order']: 'ASC');
  switch (@$_GET['sort']) {
    case 'price':
      $id = 'commerce_price_amount';      
      $table = 'field_data_commerce_price';
      $label = 'Price';
      $relationship = 'field_product_product_id';
      break;

    case 'date':
      $id = 'created';      
      $table = 'node';
      $label = 'Post date';
      break;
    
    default:
      $id = 'title';
      $table = 'node';
      $label = 'Title';
      break;
  }

  $options = array(
    'order' => $order,
    'id' => $id,
  );
  if ($relationship) {
    $options['relationship'] = $relationship;
  }
  
  $view->add_item($display, 'sort', $table, $id, $options);
 
  $query = '';
  foreach ($_GET as $key => $value) {
    if ($key != 'q' && $key != 'sort' && $key != 'order') {
      $query .= ($query ? '&' : '?') . $key . '=' . $value;
    }
  }
  
  $sort_options = array('title' => 'Title', 'price' => 'Price', 'date' => 'Post date');
  $start_url = $_SERVER['REDIRECT_URL'] . $query . ($query ? '&' : '?');
  
  $sort_exposed = '';
  
  foreach($sort_options as $sort_option => $sort_title) {
    $order_marker = '';
    if ($sort_option == @$_GET['sort']) {
      if (!@$_GET['order'] || $_GET['order'] == 'DESC') {
        $current_order = 'ASC';
        $order_marker = '-down-';
      }
      else {
        $current_order = 'DESC';
        $order_marker = '-up-';
      }
    }
    else {
      $current_order = 'ASC';
    }
    $sort_exposed .= ' | <a href="' . $start_url . 'sort=' . $sort_option . '&order=' . $current_order . '">' . $sort_title . $order_marker . '</a>';
  }
  
  echo $sort_exposed;
  echo $view->preview($display);
}
    
?>
    
    
<?php //hide($content['links']); //print render($content['comments']); ?>

<?php if (!$page): ?>
  </article> <!-- /.node -->
<?php endif; ?>
