#pragma once
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

namespace proton {
	 
/**
* GLOBAL
*/
struct[[eosio::table, eosio::contract("swapfees")]] global{
		global() {}
		uint64_t one_process_cycles;
		int64_t last_processed_id;
		uint64_t interval = 0;
		uint64_t last_loop_time = 0;
		uint64_t spare5 = 0;
		uint64_t spare6 = 0;
	EOSLIB_SERIALIZE(global, (one_process_cycles)(last_processed_id)(interval)(last_loop_time)(spare5)(spare6))
	};
	typedef eosio::singleton< "global"_n, global> conf;
	
	struct[[eosio::table, eosio::contract("swapfees")]] deposit{
		uint64_t id;
		checksum256 hash;
		extended_asset fee_deposit;
		symbol_code lt_symbol;

		uint64_t primary_key()const{
			return id;
		}
		checksum256 by_hash() const {
			return hash;
		}

	  EOSLIB_SERIALIZE(deposit, (id)(hash)(fee_deposit)(lt_symbol))
	};
	typedef eosio::multi_index< "deposits"_n, deposit ,
		eosio::indexed_by< "hash"_n, eosio::const_mem_fun<deposit, checksum256, &deposit::by_hash>>
	> deposits;
}