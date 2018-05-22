
#27.04.2018  Juan D. Gomez
# This code reads all the tarining and testing data from the
# "Grasp-and-Lift EEG Detection challenge" at 
# https://www.kaggle.com/c/grasp-and-lift-eeg-detection/
# The code remove the rigth hemisphere channels and classify the rest of channels signals
# into six evenets as described by the challenge itself.


#install.packages("tidyr")
#install.packages("string")
#install.packages("dplyr")
#install.packages("randomForest")
#install.packages("e1071")
#install.packages("caret")
#install.packages("neuralnet")


#set local path or data repository
setwd("C:/Users/Juan Gomez/Desktop/mind-x") 
#clean the workspace to beging with
rm(list=ls(all=TRUE)) 

#request necesary libraries
library(dplyr)
library(tidyr)
library(stringr)
library(randomForest)
library(e1071)
library(caret)
library(neuralnet)

#simple screen cleaning function (clc())
clc <- function() cat(rep("\n", 50))
clc()


# This is the central function of the code.
# It is called for every subject with his/her respective training and testing data.
# ANNs are trained with the training data and then, testing data classification is predicted. 
classifiers <- function(train,test,met){
  
  #these are the formule for the ANNs training.
  #VERY IMPORTANT!!
  #Notice that only left hemisphere channels are being used.
  #This action represents a spacial filtering of the signals.
  #The movements are all made with the right hand (contraolled by the left hem.).
  
  formula1 <- HandStart ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10   
  formula2 <- FirstDigitTouch ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10
  formula3 <- BothStartLoadPhase ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10
  formula4 <- LiftOff ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10
  formula5 <- Replace ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10
  formula6 <- BothReleased ~ Fp1+F7+F3+Fz+FC5+FC1+T7+C3+Cz+TP9+CP5+CP1+P7+P3+Pz+PO9+O1+Oz+PO10
  
  #now lets see how many samples (num) from the training set are actually taken.
  #When num=20000 are taken, the kaggle score is 84%, yet the execution time is way to long
  
  #num<- 20000# take only num
  num<- nrow(train)# take all of them
  
  
  
  if (met==0){ #go with ANNs
  
  #since many times not all the samples are taken for training but just a few
  #next three lines guarantee that the num of samples taken are random by re-ordering the data
  set.seed(9850)
  g<-runif(nrow(train))
  train<-train[order(g),]
  
  #the ANNs are trained with num samples. 
  #this samples are randomly selected since the data was ramdonly re-ordered avobe
  net1 <- neuralnet(formula1,train[1:num,], hidden=4, threshold=0.01)
  net2 <- neuralnet(formula2,train[1:num,], hidden=4, threshold=0.01)
  net3 <- neuralnet(formula3,train[1:num,], hidden=4, threshold=0.01)
  net4 <- neuralnet(formula4,train[1:num,], hidden=4, threshold=0.01)
  net5 <- neuralnet(formula5,train[1:num,], hidden=4, threshold=0.01)
  net6 <- neuralnet(formula6,train[1:num,], hidden=4, threshold=0.01)
  
  #the predictions are made with testing data
  my_predict1 <- compute(net1, test[,2:20])
  my_predict2 <- compute(net2, test[,2:20])
  my_predict3 <- compute(net3, test[,2:20])
  my_predict4 <- compute(net4, test[,2:20])
  my_predict5 <- compute(net5, test[,2:20])
  my_predict6 <- compute(net6, test[,2:20])
   
  #predictions are put together in a single frame 
  my_solution <- data.frame(test$id,my_predict1$net.result,my_predict2$net.result,my_predict3$net.result,
                            my_predict4$net.result,my_predict5$net.result,my_predict6$net.result)
  
  
  }
  
  if (met==1){ #Go with regression. Notice that all the data get used when regression is applied
    
    reg_model1 <- glm(formula1, data=train,family=binomial)
    reg_model2 <- glm(formula2, data=train,family=binomial)
    reg_model3 <- glm(formula3, data=train,family=binomial)
    reg_model4 <- glm(formula4, data=train,family=binomial)
    reg_model5 <- glm(formula5, data=train,family=binomial)
    reg_model6 <- glm(formula6, data=train,family=binomial)
    
    my_predict1 <- predict(reg_model1,newdata=test,type='response')
    my_predict2 <- predict(reg_model2,newdata=test,type='response')
    my_predict3 <- predict(reg_model3,newdata=test,type='response')
    my_predict4 <- predict(reg_model4,newdata=test,type='response')
    my_predict5 <- predict(reg_model5,newdata=test,type='response')
    my_predict6 <- predict(reg_model6,newdata=test,type='response')
    
    my_solution <- data.frame(test$id,my_predict1,my_predict2,my_predict3,
                              my_predict4,my_predict5,my_predict6)
    
  }
  
  
  my_solution
  
  
}


#this function merges data with events
merging <- function (eeg,events){
  # merge events with data
  data <- merge(eeg,events,'id')
  data$value <- NULL
  #seperate id into frame number and id
  data <- separate(data,id,c('subject','series','frame'),sep='_')
  data$frame <- as.numeric(data$frame)
  # order based on id
  data <- data[order(data$frame),]
  #data$seriesno <- NULL
  data <- unite(data,col=id,subject,series,frame,sep='_',remove = TRUE)
  data
}



set.seed(100)
options(scipen=999) #no scientific numbers
options (digits = 4) #hold decimal places to 4
# number of subjects you want to analyze
total_subj <- 12
# sub-sample training data to reduce computational load (min 1, max 8)
sub_sample <- 8
#initialize list
list_subj_predictions <- list()

# Loop through subjects
for (j in 1:total_subj){
    print(paste('Running subject number',j))
    list_subj_traindata <- list() # initialize list   
    subject <- j
    # obtain all training data
    for (i in 1:sub_sample) {
        file_name_eeg <- paste('subj',subject,'_series',i,
                               '_data.csv',sep='') 
        file_name_events <- paste('subj',subject,'_series',i,
                                  '_events.csv',sep='') 
        eeg <- read.csv(file_name_eeg, header=TRUE,
                        colClasses=c("character",rep("numeric",32)))#read data.csv based on file name
        events <- read.csv(file_name_events,header=TRUE,
                           colClasses=c("character",rep("numeric",6))) #read events.csv based on file name
        list_subj_traindata[[i]]<-merging(eeg,events)
        rm(eeg,events)
    }
    #merge all training series data in 1 single data frame per subject
    train_variable_name <- paste('subj',subject,'_traindata',sep='')
    assign(train_variable_name,rbind_all(list_subj_traindata))
    rm(list_subj_traindata)

    #obtain and merge test data from all series    
    list_subj_testdata <- list()
    count <- 0
    # i is series number
    for (i in 9:10) {
        count <- count + 1 
        file_name_eeg <- paste('subj',subject,'_series',i,
                               '_data.csv',sep='')
        eeg <- read.csv(file_name_eeg, header=TRUE,
                        colClasses=c("character",rep("numeric",32)))
        list_subj_testdata[[count]]<- eeg
        rm(eeg)
    }
    #merge all series data in 1 single data frame per subject
    test_variable_name <- paste('subj',subject,'_testdata',sep='')
    assign(test_variable_name,rbind_all(list_subj_testdata))
    rm(list_subj_testdata)    
    
   
    
    #Classification
    print('classification going on')
    #debug(glm_regression)
    list_subj_predictions[[j]]<- classifiers(get(train_variable_name),
                                                get(test_variable_name),0)
    
    rm(list=ls(pattern="^subj")) #remove variables in workspace to reduce memory usage
    
    
}

# combine all subject predictions into 1 dataframe for a solution
solution <- rbind_all(list_subj_predictions)
solution[is.na(solution)] <- 0
names(solution) <- c('id','HandStart','FirstDigitTouch',
                          'BothStartLoadPhase','LiftOff','Replace','BothReleased')
solution[,2:ncol(solution)]= round(solution[,2:ncol(solution)],digits=4) # reduce file size by limiting number of decimal places

write.csv(solution,file='SolutionANNs.csv',row.names=FALSE)

