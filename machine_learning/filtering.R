#27.04.2018  Juan D. Gomez
# This code reads all the tarining and testing data from the
# "Grasp-and-Lift EEG Detection challenge" at 
# https://www.kaggle.com/c/grasp-and-lift-eeg-detection/
# The code filter the signals using a butterworth filter




#install.packages("tidyr")
#install.packages("string")
#install.packages("dplyr")
#install.packages("signal")


#set local path or data repository
setwd("C:/Users/Juan Gomez/Desktop/mind-x") 
#clean the workspace to beging with
rm(list=ls(all=TRUE)) 

#request necesary libraries
library(dplyr)
library(tidyr)
library(stringr)
library(signal)

#simple screen cleaning with clc()
clc <- function() cat(rep("\n", 50))
clc()

set.seed(100)
options(scipen=999) #no scientific numbers
options (digits = 4) #hold decimal places to 4
# number of subjects you want to analyze
total_subj <- 12
# sub-sample training data to reduce computational load (min 1, max 8)
sub_sample <- 10
#50 Hz filter
W <- 1/50
b1 <- butter(2, 1/50, type="low")

# Loop through subjects
for (j in 1:total_subj){
  print(paste('Currently filtering subject',j))
  subject <- j
  # obtain all training data
  for (i in 1:sub_sample) {
    print(paste('...series',i))
    
    file_name_eeg <- paste('subj',subject,'_series',i,
                           '_data.csv',sep='') 
  
    eeg <- read.csv(file_name_eeg, header=TRUE,
                    colClasses=c("character",rep("numeric",32)))#read data.csv based on file name
    
    #filtering
    
    for (ch in 2:ncol(eeg)){#go through channels
      eegF <- filtfilt(b1, eeg[, ch])#extract raw signal (to be filtered) from data frame
      eeg[, ch]<-eegF#return filterd signal into data frame
      
    }
    #rewriting the file, now with a filtered signals
    write.csv(eeg,file=file_name_eeg,row.names=FALSE)

  }
  clc()
}







