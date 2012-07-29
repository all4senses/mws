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
      <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>

    
    
    
  <?php 
  
  print '<div>Test</div>'; 
  
  //print fboauth_action_display('connect', 'http://mws.all4senses.com' . $_SERVER['REQUEST_URI']);  
  $link = fboauth_action_link_properties('connect', 'http://mws.all4senses.com' . $_SERVER['REQUEST_URI']);
  print l(t('Login via Facebook'), $link['href'], array('query' => $link['query']));

  if (isset($_SESSION['fb_longLiveToken'])) {
    echo '<div>fb_id = ' . $_SESSION['fb_id'] . '</div><div>fb_longLiveToken = ' . $_SESSION['fb_longLiveToken'] . '</div>';
    
    mws_fb_photoImport($_SESSION['fb_id'], $_SESSION['fb_longLiveToken']);
  }
  

  
  $url = 'http://mws.all4senses.com' . ($_SERVER['REQUEST_URI'] == '/home' ? '/' : $_SERVER['REQUEST_URI']);
  
  
  ?>
    
<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/ru_RU/all.js#xfbml=1&appId=271416119626001";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>  

<div class="fb-comments" data-href="<?php echo $url; ?>" data-num-posts="2" data-width="470"></div>



  <?php print render($content['comments']); ?>

<?php if (!$page): ?>
  </article> <!-- /.node -->
<?php endif; ?>
