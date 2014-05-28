
means <- function(data) {

    r <- data.frame(data$n)
    
    len <- ncol(data)
    vi <- c(F, 2:l %% 3 == 2)
    vm <- c(F, 2:l %% 3 == 0)
    vq <- c(F, 2:l %% 3 == 1)

    i <- as.numeric(rowMeans(data[, vi]))
    m <- as.numeric(rowMeans(data[, vm]))
    q <- as.numeric(rowMeans(data[, vq]))

    r <- cbind(r, i)
    r <- cbind(r, m)
    r <- cbind(r, q)

    names(r) <- c("n", "insertionsort", "mergesort", "quicksort")

    r
}

plot.means <- function(data, inter) {

    r <- means(data)
    plot(
