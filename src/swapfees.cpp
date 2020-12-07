#include <swapfees.hpp>

namespace proton {

#ifdef DEBUG

	ACTION swapfees::changesymbol(extended_symbol deposit_symbol, symbol_code new_lt_symbol){
		require_auth(get_self());

		auto index_hash = deposits_.get_index<"hash"_n>();
		auto it_hash = index_hash.find(make_hash256(deposit_symbol));

		// If not found, initialize
		check(!(it_hash == index_hash.end()), "deposit symbol " + deposit_symbol.get_contract().to_string() + " " + deposit_symbol.get_symbol().code().to_string() + " does not exist in deposit table");

		auto itr = deposits_.require_find(it_hash->id, string("deposit with symbol " + deposit_symbol.get_symbol().code().to_string() + " does not exist").c_str());

		deposits_.modify(itr, get_self(), [&](auto& s) {
			s.lt_symbol = new_lt_symbol;
		});
	}

	ACTION swapfees::clean() {
		require_auth(get_self());

		for (auto itr = deposits_.begin(); itr != deposits_.end();)
		{
			itr = deposits_.erase(itr);
		}
	}
#endif
	void swapfees::send_to_exchange(extended_asset to_exchange, string memo)
	{
		if (to_exchange.quantity.amount > 0)
		{
			action(
				permission_level{ get_self(),"active"_n },
				to_exchange.contract,
				"transfer"_n,
				make_tuple(get_self(), EXCHANGE_CONTRACT, to_exchange.quantity, memo)
			).send();
		}
	}

	void swapfees::send_to_eosio(asset quantity)
	{
		if (quantity.amount > 0 && is_account(EOSIO_CONTRACT))
		{
			const string memo = "spf";
			
			action(
				permission_level{ get_self(),"active"_n },
				EOSIO_TOKEN,
				"transfer"_n,
				make_tuple(get_self(), EOSIO_CONTRACT, quantity, memo)
			).send();
		}
	}

	ACTION swapfees::process() {
		require_auth(get_self());

		const auto last_processed_id = get_last_processed_id();
		const auto now = current_time_point().sec_since_epoch();

		if ((last_processed_id == NO_ID) && (now - get_last_loop_time() < get_interval()))
		{
			check(false, "Next process possible in " + to_string(now - get_last_loop_time() - get_interval()) + " seconds");
		}

		set_last_loop_time(now);

		auto itr = deposits_.begin();
		// if deleted or empty deposit
		if (last_processed_id != NO_ID)
		{
			itr = deposits_.find(last_processed_id);
		}

		uint64_t count = 0;

		const auto one_process_cycles = get_one_process_cycle();

		while (itr != deposits_.end() && count <= one_process_cycles) {
			count++;
			if (itr->fee_deposit.contract == EOSIO_TOKEN
				&& itr->fee_deposit.get_extended_symbol().get_symbol().code().to_string() == XPR)
			{
				send_to_eosio(itr->fee_deposit.quantity);

				deposits_.modify(itr, get_self(), [&](auto& s) {
					s.fee_deposit.quantity.amount = 0;
				});
			}
			else
			{
				// if lt symbol has conversion to XPR then do convertion else skip
				if (itr->lt_symbol != symbol_code())
				{
					send_to_exchange(itr->fee_deposit, itr->lt_symbol.to_string() + ",1");

					deposits_.modify(itr, get_self(), [&](auto& s) {
						s.fee_deposit.quantity.amount = 0;
					});
				}
			}

			// save last processed symbol
			set_last_processed_id(itr->id);

			++itr;
			if (itr == deposits_.end())
			{
				// end of procesing loop
				set_last_processed_id(NO_ID);
				break;
			}
		}
	}

	checksum256 swapfees::make_hash256(extended_symbol deposit_symbol)
	{
		string index = deposit_symbol.get_contract().to_string() + "," +
			deposit_symbol.get_symbol().code().to_string();

		return sha256(index.c_str(), index.size());
	}

	checksum256 swapfees::make_hash256(extended_asset deposit_asset)
	{
		return make_hash256(deposit_asset.get_extended_symbol());
	}

}