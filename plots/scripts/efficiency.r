library(ggplot2)

data = read.csv("../strongscaling.csv")

data$Fragments = as.factor(data$Fragments)
str(data)

ggplot(data, aes(x = Processors, y = Efficiency.S, color = Fragments)) + 
  geom_point(size=2) + geom_line(size = 1) + 
  scale_x_log10(breaks = c(128, 256, 512, 1024, 2048, 4096, 8192, 16384)) + 
  #scale_y_continuous(breaks = c(1, 2, 3, 4, 5)) +
  xlab("Number of Processors") +  
  ylab("Efficiency") + 
  #ggtitle("") +
  theme_bw()
