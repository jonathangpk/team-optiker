import { AppBar, BottomNavigation, BottomNavigationAction, Box, IconButton, Paper, Typography } from "@mui/material";
import { Container } from "../components/Container";
import { ListingsComponent } from "../components/ListingComponent";
import { PriceGraph } from "../components/NewChart";
import { OrderOverview } from "../components/OrderOverview";



export function AdminPage() {
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
          <PriceGraph />
        }
      </Box>
      <Paper sx={{ position: 'fixed', bottom: 0, left: 0, right: 0 }} elevation={3}>
        <BottomNavigation
          showLabels={true}
        >
          <BottomNavigationAction label="Home" value="home" />
          <BottomNavigationAction label="Graphs" value="graphs" />
          <BottomNavigationAction label="Orders" value="orders" />
        </BottomNavigation>
      </Paper>
    </Box>
  );
}