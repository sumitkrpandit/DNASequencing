require(ggplot2)

plot <- ggplot(data = data.frame(x=0), mapping = aes(x=x))  #dummy data set

  fun.1 <- function(x) (129*x^2 - 4 * x)/1024^3   #old
  
  fun.2 <- function(x) (8*x^2 + 121 * x)/1024^3   #new
 
  plot + stat_function(fun = fun.1, size = 1, aes(colour = "REQULAR"))  + 
    stat_function(fun = fun.2, size = 1, aes(colour = "INDEXED")) + 
    scale_x_continuous(limits = c(0,80000)) +
    scale_color_manual(name = "Implementation", values = c("red", "blue")) +
    theme_bw() +
    xlab("DNA Fragments (~125bps)") +
    ylab("Memory Usage (GBs)") +
    ggtitle("                    Theoretical Memory Footprint") 

   


#  plot + layer(stat = "function", fun = fun.1, mapping = aes(color = "fun.1")) + 
  
#  layer(stat = "function", fun = fun.2, mapping = aes(color = "fun.2")) + 
  
#  scale_x_continuous(limits = c(0, 1000))  +
  
#  scale_color_manual(name = "Function", values = c("blue", "red"), labels = c("OLD", "NEW"))
