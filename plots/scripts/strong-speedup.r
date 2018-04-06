library(ggplot2)

data = read.csv("../strongscaling.csv")

data$Fragments = as.factor(data$Fragments)

ggplot(data, aes(x = Processors, y = Speedup, color = Fragments)) + 
  geom_point(size=3, aes(shape = Fragments)) + 
  geom_line(size = 1) + 
  scale_shape_manual(values  = c(15, 19, 17, 8)) +
  scale_x_log10(breaks = c(128, 256, 512, 1024, 2048, 4096, 8192, 16384)) + 
  #scale_y_continuous(breaks = c(1, 2, 3, 4, 5)) +
  xlab("Number of Cores") + 
  #ggtitle("") +
  theme_bw()
  
ggsave("../../strong-speedup.pdf", last_plot())