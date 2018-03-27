library(ggplot2)

data = read.csv("../weakscaling.csv")


ggplot(data, aes(x = Processors, y = Efficiency)) + 
  geom_point(size=2, color = "red") + geom_line(size = 1, color = "red") + 
  scale_x_log10(breaks = c(64,128, 256, 512, 1024, 2048, 4096, 8192, 16384)) + 
  #scale_y_continuous(breaks = c(1, 2, 3, 4, 5)) +
  xlab("Number of Cores") +  
  #ggtitle("") +
  theme_bw()
