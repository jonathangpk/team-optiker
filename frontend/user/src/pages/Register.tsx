import { Box, Button, TextField } from "@mui/material";
import { useState } from "react";
import { useStore } from "../state/state";


export function Register() {
  const store = useStore();
  const [name, setName] = useState('');

  const onRegister = () => {
    store.register(name);
  }
  return (
    <Box>
      <TextField value={name} onChange={e => setName(e.target.value)} label={'Name'} />
      <Button onClick={onRegister}>Sign Up</Button>
    </Box>
  )
}