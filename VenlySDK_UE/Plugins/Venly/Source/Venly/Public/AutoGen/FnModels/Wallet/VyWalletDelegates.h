#pragma once

#include "CoreMinimal.h"

struct FVyGetWalletsResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetWalletsComplete, FVyGetWalletsResponse);

struct FVyCreateWalletResponse;
DECLARE_DELEGATE_OneParam(FVyOnCreateWalletComplete, FVyCreateWalletResponse);

struct FVyExportWalletResponse;
DECLARE_DELEGATE_OneParam(FVyOnExportWalletComplete, FVyExportWalletResponse);

struct FVyImportWalletKeyStoreResponse;
DECLARE_DELEGATE_OneParam(FVyOnImportWalletKeyStoreComplete, FVyImportWalletKeyStoreResponse);

struct FVyImportWalletPrivateKeyResponse;
DECLARE_DELEGATE_OneParam(FVyOnImportWalletPrivateKeyComplete, FVyImportWalletPrivateKeyResponse);

struct FVyImportWalletWifResponse;
DECLARE_DELEGATE_OneParam(FVyOnImportWalletWifComplete, FVyImportWalletWifResponse);

struct FVyImportWalletWifPassphraseResponse;
DECLARE_DELEGATE_OneParam(FVyOnImportWalletWifPassphraseComplete, FVyImportWalletWifPassphraseResponse);

struct FVyImportWalletMigrationResponse;
DECLARE_DELEGATE_OneParam(FVyOnImportWalletMigrationComplete, FVyImportWalletMigrationResponse);

struct FVyResubmitTransactionResponse;
DECLARE_DELEGATE_OneParam(FVyOnResubmitTransactionComplete, FVyResubmitTransactionResponse);

struct FVyExecuteContractResponse;
DECLARE_DELEGATE_OneParam(FVyOnExecuteContractComplete, FVyExecuteContractResponse);

struct FVyTransferGasResponse;
DECLARE_DELEGATE_OneParam(FVyOnTransferGasComplete, FVyTransferGasResponse);

struct FVyTransferMultiTokenResponse;
DECLARE_DELEGATE_OneParam(FVyOnTransferMultiTokenComplete, FVyTransferMultiTokenResponse);

struct FVyTransferCryptoTokenResponse;
DECLARE_DELEGATE_OneParam(FVyOnTransferCryptoTokenComplete, FVyTransferCryptoTokenResponse);

struct FVyTransferNativeTokenResponse;
DECLARE_DELEGATE_OneParam(FVyOnTransferNativeTokenComplete, FVyTransferNativeTokenResponse);

struct FVyCancelTransactionResponse;
DECLARE_DELEGATE_OneParam(FVyOnCancelTransactionComplete, FVyCancelTransactionResponse);

struct FVySignTransactionResponse;
DECLARE_DELEGATE_OneParam(FVyOnSignTransactionComplete, FVySignTransactionResponse);

struct FVyIsValidSignatureResponse;
DECLARE_DELEGATE_OneParam(FVyOnIsValidSignatureComplete, FVyIsValidSignatureResponse);

struct FVyReadContractResponse;
DECLARE_DELEGATE_OneParam(FVyOnReadContractComplete, FVyReadContractResponse);

struct FVyUpdateSecurityResponse;
DECLARE_DELEGATE_OneParam(FVyOnUpdateSecurityComplete, FVyUpdateSecurityResponse);

struct FVyGetWalletResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetWalletComplete, FVyGetWalletResponse);

struct FVyUpdateMetadataResponse;
DECLARE_DELEGATE_OneParam(FVyOnUpdateMetadataComplete, FVyUpdateMetadataResponse);

struct FVyGetMultiTokenBalancesResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetMultiTokenBalancesComplete, FVyGetMultiTokenBalancesResponse);

struct FVyGetWalletEventsResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetWalletEventsComplete, FVyGetWalletEventsResponse);

struct FVyGetNativeBalanceResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetNativeBalanceComplete, FVyGetNativeBalanceResponse);

struct FVyGetCryptoTokenBalancesResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetCryptoTokenBalancesComplete, FVyGetCryptoTokenBalancesResponse);

struct FVyGetCryptoTokenBalanceResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetCryptoTokenBalanceComplete, FVyGetCryptoTokenBalanceResponse);

struct FVyGetMultiTokenBalancesByAddressResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetMultiTokenBalancesByAddressComplete, FVyGetMultiTokenBalancesByAddressResponse);

struct FVyGetNativeBalanceByAddressResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetNativeBalanceByAddressComplete, FVyGetNativeBalanceByAddressResponse);

struct FVyGetCryptoTokenBalancesByAddressResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetCryptoTokenBalancesByAddressComplete, FVyGetCryptoTokenBalancesByAddressResponse);

struct FVyGetCryptoTokenBalanceByAddressResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetCryptoTokenBalanceByAddressComplete, FVyGetCryptoTokenBalanceByAddressResponse);

struct FVyGetTradingPairsResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetTradingPairsComplete, FVyGetTradingPairsResponse);

struct FVyGetAllMultiTokenBalancesResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetAllMultiTokenBalancesComplete, FVyGetAllMultiTokenBalancesResponse);

struct FVyGetClientsStatisticsResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetClientsStatisticsComplete, FVyGetClientsStatisticsResponse);

struct FVyIsValidWalletAddressResponse;
DECLARE_DELEGATE_OneParam(FVyOnIsValidWalletAddressComplete, FVyIsValidWalletAddressResponse);

struct FVyGetTransactionInfoResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetTransactionInfoComplete, FVyGetTransactionInfoResponse);

struct FVyGetTransactionFeesResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetTransactionFeesComplete, FVyGetTransactionFeesResponse);

struct FVyGetExchangeRateResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetExchangeRateComplete, FVyGetExchangeRateResponse);

struct FVyGetBlockchainInfoResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetBlockchainInfoComplete, FVyGetBlockchainInfoResponse);

struct FVyGetSupportedWalletChainsResponse;
DECLARE_DELEGATE_OneParam(FVyOnGetSupportedWalletChainsComplete, FVyGetSupportedWalletChainsResponse);