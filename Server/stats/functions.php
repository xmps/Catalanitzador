<?php
/**** SESSIONS ****/
function get_stacked_sessions() {
	
	global $db;
	
	$v = get_version_filter();

	if(!empty($v)) {
		$v = ' where ApplicationsID = '.$v;
	}
	
	$results = $db->get_results("select DATE_FORMAT(Date, '%Y-%m-%d') as day, count(id) as total from sessions $v group by day order by day asc;");
	
	return $results;
}

/**** OS ****/
function print_os_data($os_count) {
	$i = 0;
	$noFirst = false;
	foreach($os_count as $k => $subtotal) {
		if($noFirst) echo ",";
		$noFirst = true;
		echo '{';
		echo 'y: ',$subtotal['percent'],',';
		echo 'color: colors[',$i,'],';
		echo 'drilldown: {';
		echo 'name:',"'$k',";
		echo 'categories:[';
		$noSecond = false;
		foreach($subtotal as $j => $subsub) {
			if($j != 'count' && $j != 'percent') {
				if($noSecond) echo ",";
				echo "'$k-$j'";
				$noSecond = true;
			}
		}
		if($k == 'Altres') echo "'Altres'";
		echo '],';
		echo 'data:[';
		$noSecond = false;
		foreach($subtotal as $j => $subsub) {
			if($j != 'count' && $j != 'percent') {
				if($noSecond) echo ",";
				echo $subsub['percent'];
				$noSecond = true;
			}
		}
		if($k == 'Altres') echo $subtotal['percent'];
		echo '],color:colors[',$i,']';
		echo '} }';
		$i++;
	}
}

function get_os_stats() {
	global $db;
	global $vselected;
	
	$where = get_version_filter();
	
	if(!empty($where)) {
		$where = ' AND ApplicationsID = '.$where;
	}
	
	$results = $db->get_results("select count(sessions.ID) as total, operatings.Name, operatings.Bits, operatings.SPMajorVersion from sessions, operatings where operatings.ID = sessions.OperatingsID ". $where ." group by operatings.Name, operatings.Bits, operatings.SPMajorVersion;");

	$total_os = 0;
	$os_count = array();

	foreach($results as $k => $result) {
		$bits = $result->Bits.'bits';
		$sp = 'SP'.$result->SPMajorVersion;
		$total_v = $result->total;	
		$total_os += $total_v;
		if(!isset($os_count[$result->Name])) {
			$os_count[$result->Name] = array();
			$os_count[$result->Name]['count']=0;
			$os_count[$result->Name]['percent']=0;
		}
		if(!isset($os_count[$result->Name][$bits.'-'.$sp])) {
			$os_count[$result->Name][$bits.'-'.$sp]=array();
			$os_count[$result->Name][$bits.'-'.$sp]['count']=0;
			$os_count[$result->Name][$bits.'-'.$sp]['percent']=0;
		}
		
		$os_count[$result->Name]['count'] += $total_v;
		$os_count[$result->Name][$bits.'-'.$sp]['count'] += $total_v;
	}

	foreach($os_count as $k => $subtotal) {

		if($k == '') {
			$os_count['Altres']['percent'] = ($subtotal['count']/$total_os)*100;
			$os_count['Altres']['count'] = $subtotal['count'];
			unset($os_count[$k]);
		} elseif($k!='Altres') {
			$os_count[$k]['percent'] = ($subtotal['count']/$total_os)*100;
			foreach($subtotal as $j=>$subsub) {
				if($j != 'percent' && $j != 'count') {
					$os_count[$k][$j]['percent']=($os_count[$k][$j]['count']/$total_os)*100;
				}
			}	
		}
	}	
	
	return $os_count;
}

/**** OFFICE ****/
function get_office_stats() {
	return get_action_stats(2);
}

function print_office_data() {
	global $office_count;
	if($office_count) {
		print_action_data($office_count);
	} else {
		print_action_data(get_office_stats());
	}
}

/**** ACTIONS DATA ****/
function get_actions_data($action_id) {
	global $db;
	
	$v = get_version_filter();

	if(!empty($v)) {
		$v = ' AND ApplicationsID = '.$v;
		$results = $db->get_results("select ActionID, Result, count(*) as total from actions a inner join sessions s on s.ID = a.SessionID where ActionID = $action_id $v group by a.ActionID , Result order by Result asc");
	} else {
		$results = $db->get_results("select ActionID, Result, count(*) as total from actions a where ActionID = $action_id group by a.ActionID , Result order by Result asc");
	}
	
	return $results;
}

function print_char($action_id,$small,$nice) {
	?>
		<?=$small?>Char = new Highcharts.Chart({
				chart: {
						renderTo: '<?=$small?>_versions',
						plotBackgroundColor: null,
						plotBorderWidth: null,
						plotShadow: false
				},
				title: {
						text: "Versions de<?=$nice?>"
				},
				tooltip: {
						formatter: function() {
								return '<b>'+ this.point.name +'</b><br />'
								+ 'Total: ' + this.y + ' (' +
								(Math.round(this.percentage*100)/100.0) +'%)';
						}
				},
				plotOptions: {
						pie: {
								allowPointSelect: true,
								cursor: 'pointer',
								dataLabels: {
										enabled: true,
										color: '#000000',
										connectorColor: '#000000'
								}
						}
				},
				series: [{ type: 'pie', name: "Versions de<?=$nice?>", data: [<?php 
					print_action_data(get_action_stats($action_id));
				?>]}]
		});	
	<?php
}


/**** GENERIC FUNCTIONS ****/
function get_version_filter() {
	global $vselected;
	global $db;
	$where = '';
	if($vselected) {
		$v = $_GET['v'];
		if(!empty($v) && is_numeric($v)) {
			$version = $db->get_results("select ID from applications "
				. " where MajorVersion = $v[0] and MinorVersion = $v[1] and "
				. " Revision = $v[2] ");
			$where = $version[0]->ID;
		}
	}
	
	return $where;
}

function get_versions() {
	global $db;
	$results = $db->get_results("select ApplicationsID, count(*) as total from sessions group by ApplicationsID;");
	$r_versions = $db->get_results("select * from applications order by ID asc");
	$versions = array();
	$totals = array();
	foreach($r_versions as $k => $version) {
		$versions["ID-".$version->ID] = $version->MajorVersion.'.'.$version->MinorVersion.'.'.$version->Revision;
		$totals["ID-".$version->ID] = 0;
	}
	
	foreach($results as $k => $result) {
		$totals["ID-".$result->ApplicationsID] = $result->total;
	}
	
	asort($versions);
	
	return array($versions,$totals);
}


function get_action_stats($action_id) {
	global $db;
	global $subversions;
	
	$total_action = 0;
	$action_count = array();

	$v = get_version_filter();

	if(in_array($action_id,$subversions)) {
		$tversion = "SUBSTRING(Version,1,LOCATE('.',Version,LOCATE('.',Version)+1)-1)";
	} else {
		$tversion = "Version";
	}

	// aquest IF no és necessari però és més eficient quan no hi ha filtre
	if(!empty($v)) {
		$v = ' AND ApplicationsID = '.$v;
		$results = $db->get_results("select count(*) total, $tversion AS NVersion from actions a inner join sessions s on s.ID = a.SessionID  where ActionID = $action_id $v group by NVersion;");
	} else {
		$results = $db->get_results("select count(*) total, $tversion AS NVersion from actions where ActionID = $action_id group by NVersion;");
	}
	
	foreach($results as $k => $result) {
		$action_count[$result->NVersion] = $result->total;
		$total_action += $result->total;
	}
	return $action_count;
}



function print_action_data($action_count) {
	$i = 0;
	foreach ( $action_count as $n=>$action ) {
		if($i!=0) echo ','; 
		$i++;
		if($n=='') $n = 'Sense programa';
		echo '["',$n,'", ',$action,']';
	}
}
?>
