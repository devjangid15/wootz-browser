// Copyright (c) 2022 The Wootz Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// Types
import { WootzWallet } from '../../../constants/types'
import { ParsedTransaction } from '../../../utils/tx-utils'

// utils
import Amount from '../../../utils/amount'
import { getLocale } from '../../../../common/locale'

// hooks
import {
  useGetDefaultFiatCurrencyQuery //
} from '../../../common/slices/api.slice'

// style
import {
  TransactionTitle,
  TransactionTypeText,
  TransactionText,
  Divider,
  SectionRow,
  SectionColumn,
  EditButton
} from './style'
import { WarningBoxTitleRow } from '../shared-panel-styles'
import { Skeleton } from '../../shared/loading-skeleton/styles'
import { Column } from '../../shared/style'

interface TransactionInfoProps {
  onToggleEditGas?: () => void
  transactionDetails: ParsedTransaction | undefined
  isZCashTransaction: boolean
  isBitcoinTransaction: boolean
  isERC721SafeTransferFrom: boolean
  isERC721TransferFrom: boolean
  transactionsNetwork: WootzWallet.NetworkInfo | undefined
  hasFeeEstimatesError: boolean
  isLoadingGasFee: boolean
  gasFee: string
  insufficientFundsError: boolean
  insufficientFundsForGasError: boolean

  // ERC20 Approve
  isERC20Approve?: boolean
  currentTokenAllowance?: string
  isCurrentAllowanceUnlimited?: boolean
}

export const TransactionInfo = ({
  onToggleEditGas,
  transactionDetails,
  isERC721SafeTransferFrom,
  isERC721TransferFrom,
  transactionsNetwork,
  hasFeeEstimatesError,
  isLoadingGasFee,
  gasFee,
  insufficientFundsError,
  insufficientFundsForGasError,
  isZCashTransaction,
  isBitcoinTransaction,
  isERC20Approve,
  isCurrentAllowanceUnlimited,
  currentTokenAllowance
}: TransactionInfoProps) => {
  // queries
  const {
    isLoading: isLoadingDefaultFiatCurrency,
    data: defaultFiatCurrency = 'usd'
  } = useGetDefaultFiatCurrencyQuery()

  // exit early if no details
  if (!transactionDetails) {
    return null
  }

  const isLoadingGasFeeFiat = isLoadingDefaultFiatCurrency || isLoadingGasFee

  const { isSolanaTransaction, isFilecoinTransaction } = transactionDetails
  const feeLocale =
    isSolanaTransaction || isZCashTransaction || isBitcoinTransaction
      ? 'wootzWalletConfirmTransactionTransactionFee'
      : 'wootzWalletConfirmTransactionGasFee'

  // render
  return (
    <>
      {isFilecoinTransaction ? (
        <>
          {transactionDetails.gasPremium && (
            <SectionColumn>
              <TransactionTitle>
                {getLocale('wootzWalletTransactionGasPremium')}
              </TransactionTitle>
              <TransactionTypeText>
                {transactionsNetwork &&
                  new Amount(transactionDetails.gasPremium)
                    .divideByDecimals(transactionsNetwork.decimals)
                    .formatAsAsset(6, transactionsNetwork.symbol)}
              </TransactionTypeText>
            </SectionColumn>
          )}

          {transactionDetails.gasLimit && (
            <SectionColumn>
              <TransactionTitle>
                {getLocale('wootzWalletTransactionGasLimit')}
              </TransactionTitle>
              <TransactionTypeText>
                {transactionsNetwork &&
                  new Amount(transactionDetails.gasLimit)
                    .divideByDecimals(transactionsNetwork.decimals)
                    .formatAsAsset(6, transactionsNetwork.symbol)}
              </TransactionTypeText>
            </SectionColumn>
          )}

          {transactionDetails.gasFeeCap && (
            <SectionColumn>
              <TransactionTitle>
                {getLocale('wootzWalletTransactionGasFeeCap')}
              </TransactionTitle>
              <TransactionTypeText>
                {transactionsNetwork &&
                  new Amount(transactionDetails.gasFeeCap)
                    .divideByDecimals(transactionsNetwork.decimals)
                    .formatAsAsset(6, transactionsNetwork.symbol)}
              </TransactionTypeText>
            </SectionColumn>
          )}
        </>
      ) : (
        <SectionRow>
          <TransactionTitle>
            {getLocale(
              isERC20Approve ? 'wootzWalletAllowSpendTransactionFee' : feeLocale
            )}
          </TransactionTitle>

          {onToggleEditGas && (
            <EditButton onClick={onToggleEditGas}>
              {getLocale('wootzWalletAllowSpendEditButton')}
            </EditButton>
          )}
        </SectionRow>
      )}

      {hasFeeEstimatesError ? (
        <TransactionText hasError={true}>
          {getLocale('wootzWalletTransactionHasFeeEstimatesError')}
        </TransactionText>
      ) : isLoadingGasFee ? (
        <FeeSkeleton />
      ) : (
        <TransactionTypeText>
          {(transactionsNetwork &&
            new Amount(gasFee)
              .divideByDecimals(transactionsNetwork.decimals)
              .formatAsAsset(6, transactionsNetwork.symbol)) ||
            ''}
        </TransactionTypeText>
      )}

      {hasFeeEstimatesError ? null : isLoadingGasFeeFiat ? (
        <FeeSkeleton />
      ) : (
        <TransactionText>
          {new Amount(transactionDetails.gasFeeFiat).formatAsFiat(
            defaultFiatCurrency
          )}
        </TransactionText>
      )}

      {/* TODO: ERC allowance checks */}

      {!isERC20Approve && (
        <>
          <Divider />

          <WarningBoxTitleRow>
            <TransactionTitle>
              {getLocale('wootzWalletConfirmTransactionTotal')}{' '}
              {!isFilecoinTransaction && getLocale(feeLocale)}
            </TransactionTitle>
          </WarningBoxTitleRow>
          <TransactionValue
            isErc721Transfer={isERC721SafeTransferFrom || isERC721TransferFrom}
            symbol={transactionDetails.symbol}
            valueExact={transactionDetails.valueExact}
          />

          {isFilecoinTransaction ? null : hasFeeEstimatesError ? (
            <TransactionText hasError>
              {getLocale('wootzWalletTransactionHasFeeEstimatesError')}
            </TransactionText>
          ) : isLoadingGasFee ? (
            <FeeSkeleton />
          ) : (
            <TransactionTypeText>
              +{' '}
              {transactionsNetwork &&
                new Amount(gasFee)
                  .divideByDecimals(transactionsNetwork.decimals)
                  .formatAsAsset(6, transactionsNetwork.symbol)}
            </TransactionTypeText>
          )}

          {hasFeeEstimatesError ? null : isLoadingGasFeeFiat ? (
            <FeeSkeleton />
          ) : (
            <TransactionText>
              {new Amount(transactionDetails.fiatTotal).formatAsFiat(
                defaultFiatCurrency
              )}
            </TransactionText>
          )}
        </>
      )}

      {insufficientFundsForGasError && (
        <TransactionText hasError={true}>
          {getLocale('wootzWalletSwapInsufficientFundsForGas')}
        </TransactionText>
      )}
      {!insufficientFundsForGasError && insufficientFundsError && (
        <TransactionText hasError={true}>
          {getLocale('wootzWalletSwapInsufficientBalance')}
        </TransactionText>
      )}

      {isERC20Approve && (
        <>
          <Divider />

          <TransactionTitle>
            {getLocale('wootzWalletAllowSpendCurrentAllowance')}
          </TransactionTitle>
          <TransactionTypeText>
            {isCurrentAllowanceUnlimited
              ? getLocale('wootzWalletTransactionApproveUnlimited')
              : currentTokenAllowance}{' '}
            {transactionDetails.symbol}
          </TransactionTypeText>

          <Divider />

          <TransactionTitle>
            {getLocale('wootzWalletAllowSpendProposedAllowance')}
          </TransactionTitle>
          <TransactionTypeText>
            {transactionDetails.isApprovalUnlimited
              ? getLocale('wootzWalletTransactionApproveUnlimited')
              : new Amount(transactionDetails.valueExact).formatAsAsset(
                  undefined,
                  transactionDetails.symbol
                )}
          </TransactionTypeText>
        </>
      )}
    </>
  )
}

const FeeSkeleton = () => {
  return (
    <Column
      fullHeight
      fullWidth
      alignItems={'flex-start'}
      justifyContent='flex-start'
    >
      <Skeleton
        width={'40px'}
        height={'12px'}
        enableAnimation
      />
    </Column>
  )
}

const TransactionValue = ({
  isErc721Transfer,
  symbol,
  valueExact
}: {
  isErc721Transfer: boolean
  valueExact: string
  symbol: string
}) => {
  /**
   * This will need updating if we ever switch to using per-locale formatting,
   * since `.` isn't always the decimal separator
   */
  const transactionValueParts = (
    isErc721Transfer
      ? valueExact
      : new Amount(valueExact).format(undefined, true)
  ).split('.')

  /**
   * Inserts a <wbr /> tag between the integer and decimal portions of the value
   * for wrapping
   * This will need updating if we ever switch to using per-locale formatting
   */
  const transactionValueText = (
    <span>
      {transactionValueParts.map((part, i, { length }) => [
        part,
        ...(i < length - 1 ? ['.'] : []),
        <wbr key={part} />
      ])}
    </span>
  )

  // render
  return (
    <TransactionTypeText>
      {transactionValueText} {symbol}
    </TransactionTypeText>
  )
}
