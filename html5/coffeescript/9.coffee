Account = (customer, cart) ->
  ###
  SkinnyMochaHalfCaffScript Compiler v1.0
  Released under the MIT License
  ###
  @customer = customer
  @cart = cart

  $('.shopping_cart').bind 'click', (event) =>
    @customer.purchase @cart

html = """
       <strong>
         cup of coffeescript
       </strong>
       """
