(function ($) {

  Drupal.behaviors.checkout_prompt = {
    attach: function (context, settings) {
       
       $(".checkout_to_original").click(function(event){
         var go_to_shop = confirm('Are you ready to be redirected to the shop site to finish the purchase? (You can choose to cancel and edit the cart before we proceed)');
         if (!go_to_shop) {
          event.preventDefault();
         }
       });

       
    }
  };

}(jQuery));
