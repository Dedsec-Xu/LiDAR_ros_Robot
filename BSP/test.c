HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  /* Check that a Tx process is not already ongoing */
  if (huart->gState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(huart);

    huart->pTxBuffPtr  = pData;
    huart->TxXferSize  = Size;
    huart->TxXferCount = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_BUSY_TX;

    if (huart->hdmatx != NULL)
    {
      /* Set the UART DMA transfer complete callback */
      huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;

      /* Set the UART DMA Half transfer complete callback */
      huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;

      /* Set the DMA error callback */
      huart->hdmatx->XferErrorCallback = UART_DMAError;

      /* Set the DMA abort callback */
      huart->hdmatx->XferAbortCallback = NULL;

      /* Enable the UART transmit DMA channel */
      if (HAL_DMA_Start_IT(huart->hdmatx, (uint32_t)huart->pTxBuffPtr, (uint32_t)&huart->Instance->TDR, Size) != HAL_OK)
      {
        /* Set error code to DMA */
        huart->ErrorCode = HAL_UART_ERROR_DMA;

        /* Process Unlocked */
        __HAL_UNLOCK(huart);

        /* Restore huart->gState to ready */
        huart->gState = HAL_UART_STATE_READY;

        return HAL_ERROR;
      }
    }
    /* Clear the TC flag in the ICR register */
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_TCF);

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the UART CR3 register */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}