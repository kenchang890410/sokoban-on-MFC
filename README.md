# 1102-210099 程式設計 Final Project: Sokoban on MFC

本專案是一個經典的推箱子遊戲 (Sokoban) 實作，採用 Microsoft Foundation Classes (MFC) 框架開發，提供了圖形使用者介面和多種互動方式。

## Demo 影片

![image](https://github.com/kenchang890410/sokoban-on-MFC/blob/10a55e21c7f88eb1161c9a69d74d1c927f4971c5/sokoban_demo.gif)

## 介面與操作說明

遊戲提供了鍵盤與滑鼠兩種控制方式，同時在介面上即時顯示遊戲狀態。

### 操作方式

鍵盤控制：玩家可使用 WASD 或 HJKL，對 Worker 進行上下左右四個方向的移動控制。

滑鼠互動：玩家可在 Worker 周圍區域點擊滑鼠左鍵，以觸發 Worker 向點擊方向移動。

### 介面資訊

遊戲介面會於右側顯示以下即時統計數據：

level：當前遊戲關卡數。

destination：地圖中總目的地的數量。

arrival：已成功推入目的地的箱子數量。

step：玩家累計的總移動步數。

## 遊戲地圖讀取與遊戲機制

遊戲地圖儲存在一系列按關卡編號命名的 .txt 檔案中，例如 map001.txt。

以下為 map001.txt 範例
```
  HHH
  HDH
  H HHHH
HHHB BDH
HD BW HH
HHHHBHH
   HDH
   HHH
```
### 地圖元素定義

H：牆壁 (wall)

W：主角/工人 (worker)

B：箱子 (box)

D：目的地 (destination)

### 核心遊戲機制

程式啟動後，會首先讀取地圖文件並將其寫入陣列，同時載入對應的圖示進行初始化顯示。
接著，遊戲會根據 Worker 的移動不斷實時更新陣列中的地圖狀態。程式的核心為偵測所有箱子是否已到達目的地，當達成此條件時，即判斷為當前關卡勝利。

## 檔案說明

整個專案因檔案數量太大，所以全部檔案包含地圖及場景圖示上傳到 Releases 的 all_file
