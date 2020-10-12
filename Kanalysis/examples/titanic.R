data <- read.csv("../external/titanic/inst/data-raw/train.csv")

dependent_var <- "Survived"
independent_vars <- c("Pclass", "Sex", "Age", "SibSp", "Parch", "Fare")
data <- data[, c(dependent_var, independent_vars)]

data <- na.omit(data)
data[, "Sex"] <- as.numeric(as.factor(data[, "Sex"])) # 1 = female, 2 = male

write.csv(data, "data/titanic.csv", row.names = FALSE)
