
load.data <- function(filename = "s.csv") {

    data <- read.csv(filename, colClasses=c("integer", "character",
                                   "character", "integer", "numeric"), quote="",
                     comment.char="")

    data$method.type <-
        sub("super_quickmergesort", "Merge.Quick.Sorted", data$method.type)
    data$method.type <-
        sub("insertionsort_w", "Insertion", data$method.type)
    data$method.type <-
        sub("quicksort_w", "Quicksort", data$method.type)
    data$method.type <-
        sub("mergesort_w", "Mergesort", data$method.type)
    data$method.type <-
        sub("merge_insertsort", "Mergesort.Insertion", data$method.type)
    data$method.type <-
        sub("quicksort_insertion", "Quicksort.Insertion", data$method.type)
    data$method.type <-
        sub("quickmergesort", "Quick.Merge.Insertion", data$method.type)
    data$method.type <-
        sub("mergecmp", "Merge.Sorted.Test", data$method.type)

    data$method.type <- factor(data$method.type,
                               labels=c("Insertion", "Quicksort", "Mergesort",
                                   "Quicksort.Insertion", "Mergesort.Insertion",
                                   "Merge.Sorted.Test", "Quick.Merge.Insertion",
                                   "Merge.Quick.Sorted"))
    
    data$insertion.threshold <- as.factor(data$insertion.threshold)

    data$sort.factor <- factor(data$sort.factor,
                               levels=c("0.01", "0.02", "0.03", "0.04", "0.05",
                                   "0.06", "0.07", "0.08", "0.09", "0.1", "0.2",
                                   "0.3", "0.4", "0.5", "0.6", "0.7", "0.8",
                                   "0.9", "0.99", "0.999", "1"),
                               labels=c("1 %", "2 %", "3 %", "4 %", "5 %",
                                   "6 %", "7 %", "8 %", "9 %", "10 %", "20 %",
                                   "30 %", "40 %", "50 %", "60 %", "70 %",
                                   "80 %", "90 %", "99 %", "99.9 %", "100 %"))

    data
}

means <- function(threshold, sortfactor) {

    
}
