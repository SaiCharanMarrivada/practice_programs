let powerset a =
    let rec powerset_aux subsets = function
      | [] -> subsets
      | h :: t -> let subsets' = List.map (fun subset -> h :: subset) subsets in
         powerset_aux (subsets @ subsets') t
    in powerset_aux [[]] subsets

let rec from start stop a = if start > stop then a else from start (stop - 1) (stop :: a)

let ( -- ) start stop = from start stop []
