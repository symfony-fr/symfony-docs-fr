#!/bin/bash

function sparse_checkout {
    mkdir sparse_checkout
    cd sparse_checkout
    git init
    git config core.sparsecheckout true
    git remote add -f origin http://github.com/$1/$2
    echo Resources/doc > .git/info/sparse-checkout
    git checkout master
    cd ..
    rm -rf sparse_checkout
}
