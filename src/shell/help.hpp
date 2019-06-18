//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

namespace komodo
{
    static constexpr const char *global_help_message = R"(komodo_playground cli

help ("command")
getinfo
getrawtransaction <txid> [verbose])";

    static constexpr const char *get_info_help_message = R"(getinfo
Returns an object containing various state info.

Result:
{
  "version": xxxxx,           (numeric) the server version
  "protocolversion": xxxxx,   (numeric) the protocol version
  "walletversion": xxxxx,     (numeric) the wallet version
  "balance": xxxxxxx,         (numeric) the total Komodo balance of the wallet
  "blocks": xxxxxx,           (numeric) the current number of blocks processed in the server
  "timeoffset": xxxxx,        (numeric) the time offset
  "connections": xxxxx,       (numeric) the number of connections
  "proxy": "host:port",     (string, optional) the proxy used by the server
  "difficulty": xxxxxx,       (numeric) the current difficulty
  "testnet": true|false,      (boolean) if the server is using testnet or not
  "keypoololdest": xxxxxx,    (numeric) the timestamp (seconds since GMT epoch) of the oldest pre-generated key in the key pool
  "keypoolsize": xxxx,        (numeric) how many new keys are pre-generated
  "unlocked_until": ttt,      (numeric) the timestamp in seconds since epoch (midnight Jan 1 1970 GMT) that the wallet is unlocked for transfers, or 0 if the wallet is locked
  "paytxfee": x.xxxx,         (numeric) the transaction fee set in KMD/kB
  "relayfee": x.xxxx,         (numeric) minimum relay fee for non-free transactions in KMD/kB
  "errors": "..."           (string) any error messages
}

Examples:
> komodo_playground getinfo
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getinfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:7771/)";

    static constexpr const char *get_rawtransaction_help_message = R"(getrawtransaction "txid" ( verbose )

NOTE: By default this function only works sometimes. This is when the tx is in the mempool
or there is an unspent output in the utxo for this transaction. To make it always work,
you need to maintain a transaction index, using the -txindex command line option.

Return the raw transaction data.

If verbose=0, returns a string that is serialized, hex-encoded data for 'txid'.
If verbose is non-zero, returns an Object with information about 'txid'.

Arguments:
1. "txid"      (string, required) The transaction id
2. verbose       (numeric, optional, default=0) If 0, return a string, other return a json object

Result (if verbose is not set or set to 0):
"data"      (string) The serialized, hex-encoded data for 'txid'

Result (if verbose > 0):
{
  "hex" : "data",       (string) The serialized, hex-encoded data for 'txid'
  "txid" : "id",        (string) The transaction id (same as provided)
  "version" : n,          (numeric) The version
  "locktime" : ttt,       (numeric) The lock time
  "expiryheight" : ttt,   (numeric, optional) The block height after which the transaction expires
  "vin" : [               (array of json objects)
     {
       "txid": "id",    (string) The transaction id
       "vout": n,         (numeric)
       "scriptSig": {     (json object) The script
         "asm": "asm",  (string) asm
         "hex": "hex"   (string) hex
       },
       "sequence": n      (numeric) The script sequence number
     }
     ,...
  ],
  "vout" : [              (array of json objects)
     {
       "value" : x.xxx,            (numeric) The value in KMD
       "n" : n,                    (numeric) index
       "scriptPubKey" : {          (json object)
         "asm" : "asm",          (string) the asm
         "hex" : "hex",          (string) the hex
         "reqSigs" : n,            (numeric) The required sigs
         "type" : "pubkeyhash",  (string) The type, eg 'pubkeyhash'
         "addresses" : [           (json array of string)
           "komodoaddress"          (string) Komodo address
           ,...
         ]
       }
     }
     ,...
  ],
  "vjoinsplit" : [        (array of json objects, only for version >= 2)
     {
       "vpub_old" : x.xxx,         (numeric) public input value in KMD
       "vpub_new" : x.xxx,         (numeric) public output value in KMD
       "anchor" : "hex",         (string) the anchor
       "nullifiers" : [            (json array of string)
         "hex"                     (string) input note nullifier
         ,...
       ],
       "commitments" : [           (json array of string)
         "hex"                     (string) output note commitment
         ,...
       ],
       "onetimePubKey" : "hex",  (string) the onetime public key used to encrypt the ciphertexts
       "randomSeed" : "hex",     (string) the random seed
       "macs" : [                  (json array of string)
         "hex"                     (string) input note MAC
         ,...
       ],
       "proof" : "hex",          (string) the zero-knowledge proof
       "ciphertexts" : [           (json array of string)
         "hex"                     (string) output note ciphertext
         ,...
       ]
     }
     ,...
  ],
  "blockhash" : "hash",   (string) the block hash
  "confirmations" : n,      (numeric) The number of notarized DPoW confirmations
  "rawconfirmations" : n,   (numeric) The number of raw confirmations
  "time" : ttt,             (numeric) The transaction time in seconds since epoch (Jan 1 1970 GMT)
  "blocktime" : ttt         (numeric) The block time in seconds since epoch (Jan 1 1970 GMT)
}

Examples:
> komodo-cli getrawtransaction "mytxid"
> komodo-cli getrawtransaction "mytxid" 1
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id":"komodo_playground", "method": "getrawtransaction", "params": ["mytxid", 1] }' -H 'content-type: text/plain;' http://127.0.0.1:7771/)";
}