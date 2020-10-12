# Import the data
data <- read.csv("../external/titanic/inst/data-raw/train.csv")

# Filter out the variables
y <- "Survived" # Dependent variable
x <- c("Pclass", "Sex", "Age", "SibSp", "Parch", "Fare") # Independent variables
data <- data[, c(y, x)]

# Clean the data
data <- na.omit(data)
data[, "Sex"] <- as.numeric(as.factor(data[, "Sex"])) # 1 = female, 2 = male

# Export the data
write.csv(data, "data/titanic.csv", row.names = FALSE)
