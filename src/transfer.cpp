#include <swapfees.hpp>

namespace proton {
	void swapfees::ontransfer(name from, name to, asset quantity, string memo) {
		// Prevent self-transfer
		if (to != get_self()) {
			return;
		}

		// Authorization
		require_auth(from);

		// Skip if deposit from system accounts
		if (from == "eosio.stake"_n || from == "eosio.ram"_n || from == "eosio"_n) {
			return;
		}

		// Action based on memo
		auto ext_token = extended_asset{ quantity, get_first_receiver() };

		// Spread out memo
		auto lt_symbol = symbol_code(memo);

		auto index_hash = deposits_.get_index<"hash"_n>();
		auto it_hash = index_hash.find(make_hash256(ext_token));
	
		// If not found, initialize
		if (it_hash == index_hash.end())
		{
			deposits_.emplace(get_self(), [&](auto& s) {
				s.id = deposits_.available_primary_key();
				s.fee_deposit = ext_token;
				s.lt_symbol = lt_symbol;
				s.hash = make_hash256(ext_token);
			});
		}
		// If found, Add provided tokens
		else
		{
			auto itr = deposits_.require_find(it_hash->id, string("id = " + to_string(it_hash->id) + " does not exist in deposit table").c_str());

			deposits_.modify(itr, get_self(), [&](auto& s) {
				s.fee_deposit.quantity.amount += ext_token.quantity.amount;
				if (!memo.empty() && itr->lt_symbol == symbol_code(""))	{
					s.lt_symbol = lt_symbol;
				}
			});
		}
	}
}