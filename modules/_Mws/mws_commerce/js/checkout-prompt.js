(function ($) {

  Drupal.behaviors.checkout_prompt = {
    attach: function (context, settings) {
       
       $(".checkout_to_original").click(function(event){
         alert('checkout_to_original');
         event.preventDefault();
         //return FALSE;
       });

       
    }
  };

}(jQuery));
