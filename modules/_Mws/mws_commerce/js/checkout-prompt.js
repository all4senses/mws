(function ($) {

  Drupal.behaviors.checkout_prompt = {
    attach: function (context, settings) {
       
       $(".checkout_to_original").click(function(event){
         var cancel = confirm('You will be redirected to the shop site. Do you want to edit the cart before we proceed?');
         if (cancel) {
          event.preventDefault();
         }
       });

       
    }
  };

}(jQuery));
