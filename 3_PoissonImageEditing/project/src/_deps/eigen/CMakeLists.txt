Ubpa_DownloadZip(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/common/eigen/eigen_3_3_7.zip
	eigen_3_3_7.zip
	SHA256 6d5cbc4c70a7b175439dee0942c6794eb454493d3585973f7d4976d0820321e2
)

Ubpa_AddTarget(
  TEST
  MODE INTERFACE
  ADD_CURRENT_TO NONE
  INC
    "src/_deps/eigen"
)
