function fetch_pull () {
  git fetch origin 'refs/pull/*/head:refs/pull/origin/*'
}
function checkout_pull() {
  git checkout refs/pull/origin/$1
}
function log_email() {
  git log --format=%ae | head -1
}
