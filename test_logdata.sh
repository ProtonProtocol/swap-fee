if [ ! -d logdata ]; then mkdir logdata; fi


## ----- Functions ---------------------------------------

getBalance(){
   ACC=$1;
   CONTRACT=$2;
   SYM=$3;

   balance=$(./cleos_protonTest.sh get table $CONTRACT $ACC accounts -L $SYM -U $SYM -l 1 | jq -r .rows[0].balance)
   if [[ $balance == "null" ]]; then
      diff=$(copareData "balance_${ACC}_${CONTRACT}_${SYM}" 0)
      echo "Balance for $ACC [ $CONTRACT - $SYM ] : ---"      
   else
      diff=$(copareData "balance_${ACC}_${CONTRACT}_${SYM}" $balance)
      echo "Balance for $ACC [ $CONTRACT - $SYM ] : $balance  | $diff"
   fi

}



copareData(){
  filen=$1
  param=$2

  if [ -f logdata/$filen ]; then
      lastParam=$(cat logdata/$filen)
      diff=$(echo "scale=4; $param-$lastParam" | bc)

    if (( $(echo "$diff==0" |bc -l) )); then
	   echo -ne "\e[33m ( no changes ) \e[0m"
    else
       echo $param > logdata/$filen
       if (( $(echo "$diff > 0" |bc -l) )); then
	   echo -ne "\e[32m ( +$diff ) \e[0m"
       else
	   echo -ne "\e[31m ( $diff ) \e[0m"
       fi
   fi


  else
    echo $param > logdata/$filen
    echo "-";
  fi

}

# ================================================
# ------------------------------------------------
echo "";
echo "--- TEST INFO ---";
echo "";
poolXPR_=$(./cleos_protonTest.sh get table eosio eosio globalsd | jq -r .rows[0].pool)
poolXPR=$( echo "scale=4; $poolXPR_/ 10000" | bc )
diff=$(copareData "globalsd_pool" $poolXPR)
echo "Reward Pool: $poolXPR XPR | $diff"

echo "";

getBalance "eosio.saving" "eosio.token" "XPR"
echo "";


getBalance "bohdan" "eosio.token" "XPR"
getBalance "bohdan" "faketoken" "USDT"
getBalance "bohdan" "testtoken" "TEST"
getBalance "bohdan" "testtoken" "JOKE"
getBalance "bohdan" "testtoken" "NUM"
getBalance "bohdan" "loltoken" "LOL"
getBalance "bohdan" "supertoken" "SUP"

getBalance "bohdan" "swaptest3" "XPRUSDT"
getBalance "bohdan" "swaptest3" "XPRTEST"
getBalance "bohdan" "swaptest3" "XPRNUM"
getBalance "bohdan" "swaptest3" "XPRSUP"
getBalance "bohdan" "swaptest3" "LOLTEST"
getBalance "bohdan" "swaptest3" "TESTUSDT"
getBalance "bohdan" "swaptest3" "USDTNUM"



echo "-----";
getBalance "test" "eosio.token" "XPR"
getBalance "test" "faketoken" "USDT"
getBalance "test" "testtoken" "TEST"
getBalance "test" "testtoken" "JOKE"
getBalance "test" "testtoken" "NUM"
getBalance "test" "loltoken" "LOL"
getBalance "test" "supertoken" "SUP"

getBalance "test" "swaptest3" "XPRUSDT"
getBalance "test" "swaptest3" "XPRTEST"
getBalance "test" "swaptest3" "XPRNUM"
getBalance "test" "swaptest3" "XPRSUP"
getBalance "test" "swaptest3" "LOLTEST"
getBalance "test" "swaptest3" "TESTUSDT"
getBalance "test" "swaptest3" "USDTNUM"


echo "-----";
getBalance "swaptest3" "eosio.token" "XPR"
getBalance "swaptest3" "faketoken" "USDT"
getBalance "swaptest3" "testtoken" "TEST"
getBalance "swaptest3" "testtoken" "JOKE"
getBalance "swaptest3" "testtoken" "NUM"
getBalance "swaptest3" "loltoken" "LOL"
getBalance "swaptest3" "supertoken" "SUP"


echo "-----";
getBalance "protocolfee3" "eosio.token" "XPR"
getBalance "protocolfee3" "faketoken" "USDT"
getBalance "protocolfee3" "testtoken" "TEST"
getBalance "protocolfee3" "testtoken" "JOKE"
getBalance "protocolfee3" "testtoken" "NUM"
getBalance "protocolfee3" "loltoken" "LOL"
getBalance "protocolfee3" "supertoken" "SUP"



#getBalance "" "swaptest3" "XPRUSDT"
#getBalance "" "swaptest3" "XPRTEST"
#getBalance "" "swaptest3" "XPRNUM"
#getBalance "" "swaptest3" "XPRSUP"
#getBalance "" "swaptest3" "LOLTEST"
#getBalance "" "swaptest3" "TESTUSDT"
#getBalance "" "swaptest3" "USDTNUM"
