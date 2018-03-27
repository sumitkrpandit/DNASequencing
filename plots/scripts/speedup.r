library(ggplot2)

data = read.csv("strongscaling.csv")

data$Fragments = as.factor(data$Fragments)

ggplot(data, aes(x = Processors, y = Speedup, color = Fragments)) + 
  geom_point(size=2) + geom_line(size = 1) + 
  scale_x_log10(breaks = c(128, 256, 512, 1024, 2048, 4096, 8192, 16384)) + 
  xlab("Number of Processors") +  
  ggtitle("") +
  theme_bw()
  