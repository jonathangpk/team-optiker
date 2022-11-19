import { AppBar, BottomNavigation, BottomNavigationAction, Box, IconButton, Paper, Typography } from "@mui/material";
import { useState } from "react";
import HomeIcon from '@mui/icons-material/Home';
import ShowChartIcon from '@mui/icons-material/ShowChart';
import SubjectIcon from '@mui/icons-material/Subject';
import { OrderGraph } from "../components/OrderGraph";
import { OrderListComponent } from "../components/OrderListComponent";

function renderContent(page: string) {
  switch(page) {
    case "home":
        return (
          <>
            <OrderGraph />
            <OrderListComponent />
          </>
        );

  }
  return null;
}

export function AdminPage() {
  const [page, setPage] = useState("home");

  return (
    <Box>
      <AppBar component="nav">
        <Typography variant={"h3"}>Admin</Typography>
      </AppBar>
      <Box
        style={{
          marginTop: 80,
          width: '100%',
          position: "fixed",
          justifyContent: "center"
        }}>
        {
          renderContent(page)
        }
      </Box>
      <Paper sx={{ position: 'fixed', bottom: 0, left: 0, right: 0 }} elevation={3}>
        <BottomNavigation
          showLabels={true}
          value={page}
          onChange={(_, val) => setPage(val)}
        >
          <BottomNavigationAction icon={<ShowChartIcon fontSize="large" />} label="Home" value="home" />
          {/* <BottomNavigationAction icon={<ShowChartIcon fontSize="large" />} label="Graphs" value="graphs" /> */}
          <BottomNavigationAction icon={<SubjectIcon fontSize="large" />} label="Orders" value="orders" />
        </BottomNavigation>
      </Paper>
    </Box>
  );
}