#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <tables.hpp>
#include <eosio/singleton.hpp>
#include <constants.hpp>

using namespace eosio;
using namespace std;
namespace proton {

	class[[eosio::contract("swapfees")]] swapfees : public contract{

		public:
			using contract::contract;
			// process of fee tokens deposits. some will be send for exchange some to eosio account
			ACTION process();
			using process_action = action_wrapper<"process"_n, &swapfees::process>;

#ifdef DEBUG
			ACTION clean();
			using clean_action = action_wrapper<"clean"_n, &swapfees::clean>;

			ACTION changesymbol(extended_symbol deposit_symbol, symbol_code new_lt_symbol);
			using changesymbol_action = action_wrapper<"changesymbol"_n, &swapfees::changesymbol>;
#endif
			// interval between two calls of action process
			ACTION setinterval(uint64_t interval)
			{
				require_auth(get_self());
				set_interval(interval);
			}
			using setinterval_action = action_wrapper<"setinterval"_n, &swapfees::setinterval>;
			
			// amounts of records processed during one call of process action
			ACTION setcycle(uint64_t one_process_cycles)
			{
				require_auth(get_self());
				set_one_process_cycles(one_process_cycles);
			}

			using setcycle_action = action_wrapper<"setcycle"_n, &swapfees::setcycle>;

			/**
			* check version
			*
			*/
			ACTION getversion() {
				check(false, "Version number 1.0.1, Build date: 2020-11-18 14:14");
			}
			using getversion_action = action_wrapper<"getversion"_n, &swapfees::getversion>;


			[[eosio::on_notify("*::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);

		private:
			uint64_t get_one_process_cycle() {
				state_ = config_.get_or_create(_self);
				return state_.one_process_cycles;
			}

			int64_t get_last_processed_id() {
				state_ = config_.get_or_create(_self);
				return state_.last_processed_id;
			}

			int64_t set_last_processed_id(int64_t last_processed_id) {
				state_ = config_.get_or_create(_self);
				state_.last_processed_id = last_processed_id;
				config_.set(state_, _self);
				return state_.last_processed_id;
			}

			uint64_t set_one_process_cycles(uint64_t one_process_cycles) {
				state_ = config_.get_or_create(_self);
				state_.one_process_cycles = one_process_cycles;
				config_.set(state_, _self);
				return state_.one_process_cycles;
			}

			uint64_t set_interval(uint64_t interval) {
				state_ = config_.get_or_create(_self);
				state_.interval = interval;
				config_.set(state_, _self);
				return state_.interval;
			}

			uint64_t get_interval() {
				state_ = config_.get_or_create(_self);
				return state_.interval;
			}

			uint64_t get_last_loop_time() {
				state_ = config_.get_or_create(_self);
				return state_.last_loop_time;
			}

			uint64_t set_last_loop_time(uint64_t last_loop_time)
			{
				state_ = config_.get_or_create(_self);
				state_.last_loop_time = last_loop_time;
				config_.set(state_, _self);
				return state_.last_loop_time;
			}

			checksum256 make_hash256(extended_asset deposit_asset);
			checksum256 make_hash256(extended_symbol deposit_symbol);

			void send_to_exchange(extended_asset quantity, string memo);
			void send_to_eosio(asset quantity);

			deposits deposits_ = { _self, _self.value };
			conf config_ = { _self, _self.value };
			global state_;
	};
}