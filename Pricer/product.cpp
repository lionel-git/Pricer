#include "product.h"
#include "currency_manager.h"
#include "pricer_exception.h"

product::product(date_t expiry, amount base_amount) :
	expiry_(expiry), base_amount_(base_amount), base_ccy_(currency_manager::get_currency(base_amount))
{
}

amount
product::pv(currency_code target_ccy) const
{
	return (base_amount_ * base_ccy_.getDF(expiry_)).countervalue(target_ccy);
}
